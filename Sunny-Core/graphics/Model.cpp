#include "Model.h"

#include "shaders/ShaderFactory.h"
#include "../system/VFS.h"

template<>
struct std::hash<sunny::graphics::Model::IndexSet>
{
	const size_t operator()(const sunny::graphics::Model::IndexSet& key) const
	{
		return (key.position) ^ (key.normal << 14) ^ (key.uv << 23);
	}
};

namespace sunny
{
	namespace graphics
	{
		enum OBJ_FACE : int
		{
			TRIANGLES = 6,  // %d/%d/%d %d/%d/%d %d/%d/%d
			QUADS     = 8   // %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d
		};

		struct SUNFormat
		{
			char* header = "SUNH"; //SUNH
			unsigned char nameLength;
			char* name;
			unsigned char animationLength;
			unsigned int vertexBufferSize;
			unsigned char** vertexData;
			unsigned int indexBufferSize;
			unsigned char* indexData;
			char* footer = "SUNF"; // SUNF
		};

		Model::Model(const std::string& path) : m_mesh(nullptr)
		{
			const std::string ext = utils::SplitString(path, ".")[1];
			
			if (ext == "sun") { LoadSUN(path); return; }
			if (ext == "obj") { LoadOBJ(path); return; }

			// Debug System
			std::cout << "sun, obj 파일만 로드 가능합니다." << std::endl;

			exit(1);
		}

		Model::~Model()
		{
			delete m_mesh;
		}

		unsigned char* Model::ReadBytes(FILE* file, unsigned char* buffer, unsigned int size)
		{
			fread(buffer, 1, size, file);

			return buffer;
		}

		void Model::LoadSUN(const std::string& path)
		{
			std::string physicalPath;
			VFS::Get()->ResolvePhysicalPath(path, physicalPath);
			
			SUNFormat format;

			FILE* f = fopen(physicalPath.c_str(), "rb");

			if (!f)
			{
				std::cout << "File Not Open: " << path << std::endl;
				exit(0);
			}

			// 헤더 비교
			{
				unsigned char header[4];
				ReadBytes(f, header, 4);
				if (memcmp(header, format.header, 4) != 0)
				{
					std::cout << "It is not .sun FIle(header compare) : " << path << std::endl;
					exit(0);
				}
			}

			// 이름 길이 파싱
			{
				byte buffer[1];
				ReadBytes(f, buffer, 1);
				format.nameLength = *buffer;
			}

			// 이름 파싱
			{
				format.name = new char[format.nameLength + 1];
				ReadBytes(f, (unsigned char*)format.name, format.nameLength);
				format.name[format.nameLength] = '\0';
			}

			// 애니메이션 길이 파싱
			{
				byte buffer[1];
				ReadBytes(f, buffer, 1);
				format.animationLength= *buffer;
				m_vertexDatas.resize(format.animationLength);
				format.vertexData = new unsigned char*[format.animationLength];
			}

			// 정점버퍼 크기 파싱
			{
				unsigned char buffer[4];
				ReadBytes(f, buffer, 4);
				format.vertexBufferSize = *(unsigned int*)buffer;
			}

			for (int i = 0; i < format.animationLength; ++i)
			{
				format.vertexData[i] = new unsigned char[format.vertexBufferSize];
				ReadBytes(f, format.vertexData[i], format.vertexBufferSize);
			}

			// 정점버퍼 파싱
			/*{
				format.vertexData = new unsigned char[format.vertexBufferSize];
				ReadBytes(f, format.vertexData, format.vertexBufferSize);
			}*/

			// 인덱스버퍼 크기 파싱
			{
				unsigned char buffer[4];
				ReadBytes(f, buffer, 4);
				format.indexBufferSize = *(unsigned int*)buffer;
			}

			// 인덱스버퍼 파싱
			{
				format.indexData = new unsigned char[format.vertexBufferSize];
				ReadBytes(f, format.indexData, format.indexBufferSize);
			}

			// 푸터 비교
			{
				unsigned char footer[4];
				ReadBytes(f, footer, 4);
				if (memcmp(footer, format.footer, 4) != 0)
				{
					std::cout << "It is not .sun FIle(footer compare) : " << path << std::endl;
					exit(0);
				}
			}

			fclose(f);

			ShaderFactory::Default3DShader()->Bind();
			
			directx::VertexArray* va = new directx::VertexArray();

			for (int i = 0; i < format.animationLength; ++i)
			{
				directx::VertexBuffer* buffer = new directx::VertexBuffer();
				buffer->SetData(format.vertexBufferSize, format.vertexData[i]);

				directx::BufferLayout layout;
				layout.Push<maths::vec3>("POSITION");
				layout.Push<maths::vec3>("NORMAL");
				layout.Push<maths::vec2>("TEXCOORD");
				layout.Push<maths::vec3>("BINORMAL");
				layout.Push<maths::vec3>("TANGENT");
				buffer->SetLayout(layout);

				va->PushBuffer(buffer);
			}

			directx::IndexBuffer* ib = new directx::IndexBuffer((unsigned int*)format.indexData, format.indexBufferSize / sizeof(unsigned int));

			if (format.animationLength > 0)
				m_mesh = new AnimationMesh(va, ib, format.animationLength);
			else
				m_mesh = new Mesh(va, ib);
		}

		void Model::LoadOBJ(const std::string& path)
		{
			std::string physicalPath;
			VFS::Get()->ResolvePhysicalPath(path, physicalPath);

			std::vector<std::string> lines = utils::SplitString((char*)system::FileSystem::ReadFile(physicalPath.c_str()), "\n");
			
			VertexSet inputVertices;

			std::vector<Vertex3D> vertices;
			std::vector<unsigned int> indices;
			std::unordered_map<IndexSet, int> mapping;

			unsigned int i = 0;

			for (std::string line : lines)
			{
				
				const char* cstr = line.c_str();

				if (strstr(cstr, "#") || strstr(cstr, "mtllib") || strstr(cstr, "g")) // 주석
				{
					continue;
				}
				else if (strstr(cstr, "v")) // 정점 정보
				{
					if (strstr(cstr, "vt")) // UV
					{
						maths::vec2 uv;
						int result = sscanf(cstr, "%*s %f %f", &uv.x, &uv.y);
						if (result == 0) continue;
						uv.y = 1.0f - uv.y;
						inputVertices.uvs.push_back(uv);
					}
					else if (strstr(cstr, "vn")) // 노말
					{
						maths::vec3 normal;
						int result = sscanf(cstr, "%*s %f %f %f", &normal.x, &normal.y, &normal.z);
						if (result == 0) continue;
						normal.y = normal.y * -1.0f;
						inputVertices.normals.push_back(normal);
					}
					else                        // 위치
					{
						maths::vec3 position;
						int result = sscanf(cstr, "%*s %f %f %f", &position.x, &position.y, &position.z);
						if (result == 0) continue;
						//position.z = position.z * -1.0f;
						inputVertices.position.push_back(position);
					}
				}
				else if (strstr(cstr, "f")) // 인덱스
				{
					IndexSet indexSet[4];

					unsigned int face = 0;
					std::string::size_type pos = 0;
					std::string target = "/";
					while ((pos = line.find(target, pos)) != std::string::npos) {
						++face;
						pos += target.length();
					}

					int result;

					switch (face)
					{
					case OBJ_FACE::TRIANGLES:  // 삼각형
						//result = sscanf(cstr, "%*s %d/%d/%d %d/%d/%d %d/%d/%d", &indexSet[2].position, &indexSet[2].uv, &indexSet[2].normal, &indexSet[1].position, &indexSet[1].uv, &indexSet[1].normal, &indexSet[0].position, &indexSet[0].uv, &indexSet[0].normal);
						result = sscanf(cstr, "%*s %d/%d/%d %d/%d/%d %d/%d/%d", &indexSet[0].position, &indexSet[0].uv, &indexSet[0].normal, &indexSet[1].position, &indexSet[1].uv, &indexSet[1].normal, &indexSet[2].position, &indexSet[2].uv, &indexSet[2].normal);
						if (result == 0) continue;
						InsertVertex(vertices, indices, mapping, inputVertices, indexSet[0]);
						InsertVertex(vertices, indices, mapping, inputVertices, indexSet[1]);
						InsertVertex(vertices, indices, mapping, inputVertices, indexSet[2]);
						break;
					case OBJ_FACE::QUADS:  // 사각형
						//result = sscanf(cstr, "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &indexSet[3].position, &indexSet[3].uv, &indexSet[3].normal, &indexSet[2].position, &indexSet[2].uv, &indexSet[2].normal, &indexSet[1].position, &indexSet[1].uv, &indexSet[1].normal, &indexSet[0].position, &indexSet[0].uv, &indexSet[0].normal);
						result = sscanf(cstr, "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &indexSet[0].position, &indexSet[0].uv, &indexSet[0].normal, &indexSet[1].position, &indexSet[1].uv, &indexSet[1].normal, &indexSet[2].position, &indexSet[2].uv, &indexSet[2].normal, &indexSet[3].position, &indexSet[3].uv, &indexSet[3].normal);
						if (result == 0) continue;
						InsertVertex(vertices, indices, mapping, inputVertices, indexSet[0]);
						InsertVertex(vertices, indices, mapping, inputVertices, indexSet[1]);
						InsertVertex(vertices, indices, mapping, inputVertices, indexSet[2]);

						InsertVertex(vertices, indices, mapping, inputVertices, indexSet[0]);
						InsertVertex(vertices, indices, mapping, inputVertices, indexSet[2]);
						InsertVertex(vertices, indices, mapping, inputVertices, indexSet[3]);

						break;
					}
				}
			}
			
			ShaderFactory::Default3DShader()->Bind();

			directx::VertexBuffer* buffer = new directx::VertexBuffer();
			buffer->SetData(vertices.size() * sizeof(Vertex3D), &vertices[0]);

			directx::BufferLayout layout;
			layout.Push<maths::vec3>("POSITION");
			layout.Push<maths::vec3>("NORMAL");
			layout.Push<maths::vec2>("TEXCOORD");
			layout.Push<maths::vec3>("BINORMAL");
			layout.Push<maths::vec3>("TANGENT");
			buffer->SetLayout(layout);

			directx::VertexArray* va = new directx::VertexArray();
			va->PushBuffer(buffer);

			directx::IndexBuffer* ib = new directx::IndexBuffer(&indices[0], indices.size());
			
			m_mesh = new Mesh(va, ib);
		}

		void Model::InsertVertex(std::vector<Vertex3D>& vertices, std::vector<unsigned int>& indices, std::unordered_map<IndexSet, int>& mapping, VertexSet& inputVertices, IndexSet& indexSet)
		{
			auto lookup = mapping.find(indexSet);

			if (lookup != mapping.end())
			{
				// 인덱스가 있다면 그대로 추가
				indices.push_back(lookup->second);
			}
			else
			{
				// 인덱스가 없다면 새로운 정점 정보 추가
				mapping[indexSet] = (int)vertices.size();
				indices.push_back(vertices.size());
				Vertex3D vertex = { inputVertices.position[indexSet.position - 1], inputVertices.normals[indexSet.normal - 1], inputVertices.uvs[indexSet.uv - 1], maths::vec3(1, 1, 1), maths::vec3(1, 1, 1) };
				vertices.push_back(vertex);
			}
		}
	}
}
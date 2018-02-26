#pragma once

#include "../sunny.h"
#include "../maths/maths.h"

#include "meshs/Mesh.h"
#include "meshs/AnimationMesh.h"

namespace sunny
{
	namespace graphics
	{
		class Model
		{
		private:
			Mesh* m_mesh;
			std::vector<unsigned char*> m_vertexDatas;

		public:
			Model(const std::string& path);
			~Model();

			inline Mesh* GetMesh() const { return m_mesh; }

			unsigned char* ReadBytes(FILE* file, unsigned char* buffer, unsigned int size);

		private:
			struct VertexSet
			{
				std::vector<maths::vec3> position;
				std::vector<maths::vec3> normals;
				std::vector<maths::vec2> uvs;
			};
			struct IndexSet
			{
				unsigned int position;
				unsigned int uv;
				unsigned int normal;

				bool operator==(const IndexSet& other) const
				{
					return position == other.position && uv == other.uv && normal == other.normal;
				}
			};

			friend struct std::hash<IndexSet>;

			void LoadSUN(const std::string& path);
			void LoadOBJ(const std::string& path);
			
			void InsertVertex(std::vector<Vertex3D>& vertices, std::vector<unsigned int>& indices, std::unordered_map<IndexSet, int>& mapping, VertexSet& inputVertices, IndexSet& indexSet);
		};
	}
}
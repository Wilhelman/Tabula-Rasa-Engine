#include "trMeshImporter.h"
//#include "trRenderer3D.h"

bool trMeshImporter::Awake()
{
	return true;
}

bool trMeshImporter::Start()
{
	return true;
}

bool trMeshImporter::CleanUp()
{
	return true;
}

bool trMeshImporter::Save(const char * path)
{
	/*mesh_data = new Mesh();
	uint num_indices = sizeof(uint) * mesh_data->index_size;
	uint num_vertices = sizeof(float) * mesh_data->vertex_size * 3;

	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[2] = { mesh_data->index_size, mesh_data->vertex_size };
	
	uint size = sizeof(ranges) + num_indices + num_vertices;
	
	char* data = new char[size]; // Allocate
	char* cursor = data;
	
	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	
	cursor += bytes; // Store indices
	bytes = num_indices;
	memcpy(cursor, mesh_data->indices, bytes);

	cursor += bytes; // Store vertices
	bytes = num_vertices;
	memcpy(cursor, mesh_data->vertices, bytes);

	FILE* file = fopen("myfile.bin", "wb");
	fwrite(cursor, 1, sizeof(cursor), file);
	fclose(file);

	// deleting 
	RELEASE(data);
	RELEASE(mesh_data);*/

	return true;
}

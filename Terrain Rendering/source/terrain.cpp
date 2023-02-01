#include "terrain.h"
#include "math_utils.h"

Terrain::Terrain() {
    m_shader = NULL;
}

Terrain::~Terrain() {
}

void Terrain::loadHeightMap(const char *filename) {
    m_data.heightmap = readRawFile(filename);
    m_data.size = 512;

    // create buffers
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    // create vertex data
    Vector3f *vertices = new Vector3f[m_data.size * m_data.size];
    for (int i = 0; i < m_data.size; i++) {
        for (int j = 0; j < m_data.size; j++) {
            vertices[i + j * m_data.size] = Vector3f((float)i / 512, (float)getHeight(i, j) / 512, (float)j / 512);
        }
    }

    // add vertex data to buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_data.size * m_data.size * sizeof(Vector3f), vertices, GL_STATIC_DRAW);

    // add index data to buffer
    long numQuads = (m_data.size - 1) * (m_data.size - 1);
    unsigned int *indices = new unsigned int[numQuads * 6];

    // create indices
    int index = 0;
    for (int z = 0; z < m_data.size - 1; z++) {
        for (int x = 0; x < m_data.size - 1; x++) {
            int topLeft = ((z + 1) * m_data.size) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z  * m_data.size) + x;
            int bottomRight = bottomLeft + 1;

            indices[index++] = topLeft;
            indices[index++] = bottomLeft;
            indices[index++] = topRight;
            indices[index++] = topRight;
            indices[index++] = bottomLeft;
            indices[index++] = bottomRight;
        }
    }

    // add index data to buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numQuads * 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    int pos_loc = 0;
    glEnableVertexAttribArray(pos_loc);

    size_t numFloats = 0;
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (const GLvoid*)(numFloats * sizeof(float)));
    numFloats += 3;

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Terrain::render() {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, (m_data.size - 1) * (m_data.size - 1) * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned char Terrain::getHeight(int x, int z) {
    return m_data.heightmap[x + z * m_data.size];
}

void Terrain::setScale(float scale) {
    m_scale = scale;
}

float Terrain::getScaledHeight(int x, int z) {
    return getHeight(x, z) * m_scale;
}

void Terrain::useProgram() {
    if (m_shader == NULL) {
        m_shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    }
    m_shader->use();
}

Shader *Terrain::getShader() {
    return m_shader;
}

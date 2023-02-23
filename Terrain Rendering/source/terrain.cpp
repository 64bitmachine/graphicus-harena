#include "terrain.h"
#include "math_utils.h"
#include "fractalterrain.h"

Terrain::Terrain(int size): m_size(size) {
    m_shader = NULL;
    m_scale = 4.0f;
    m_min_height = 0.0f;
    m_max_height = 300.0f;
    m_heightmap = Array2D<float>(m_size, m_size);
}

Terrain::~Terrain() {
}

void Terrain::loadHeightMap(const char *filename) {
    // m_data.heightmap = readRawFile(filename);
    m_heightmap = terrainFromFaultFormation(m_size, 500, m_max_height, m_min_height, 0.5);
    // m_data.print();

    // create buffers
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    // create vertex data
    Vector3f *vertices = new Vector3f[m_heightmap.width() * m_heightmap.height()];
    for (int i = 0; i < m_heightmap.width(); i++) {
        for (int j = 0; j < m_heightmap.height(); j++) {
            vertices[i * m_heightmap.width() + j] = Vector3f(i, m_heightmap(i, j), j);
        }
    }

    // add vertex data to buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_heightmap.width() * m_heightmap.height() * sizeof(Vector3f), vertices, GL_STATIC_DRAW);

    // add index data to buffer
    long numQuads = (m_heightmap.width() - 1) * (m_heightmap.height() - 1);
    unsigned int *indices = new unsigned int[numQuads * 6];

    // create indices
    int index = 0;
    for (int z = 0; z < m_heightmap.height() - 1; z++) {
        for (int x = 0; x < m_heightmap.width() - 1; x++) {
            int topLeft = ((z + 1) * m_heightmap.width()) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z  * m_heightmap.width()) + x;
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

    m_shader->setFloat("gMinHeight", m_min_height);
    m_shader->setFloat("gMaxHeight", m_max_height);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, (m_heightmap.width() - 1) * (m_heightmap.height() - 1) * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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
#include "terrain.h"

Terrain::Terrain() {
    m_shader = NULL;
}

Terrain::~Terrain() {
}

void Terrain::loadHeightMap(const char *filename) {
    m_data.heightmap = readRawFile(filename);
    m_data.size = 512;

    // create vao
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // create vbo
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_data.size * m_data.size * 3 * sizeof(float), NULL, GL_STATIC_DRAW);

    // create ebo
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data.size * m_data.size * 6 * sizeof(unsigned int), NULL, GL_STATIC_DRAW);

    // create vertex data
    float *vertices = new float[m_data.size * m_data.size * 3];
    for (int z = 0; z < m_data.size; z++) {
        for (int x = 0; x < m_data.size; x++) {
            vertices[(x + z * m_data.size) * 3 + 0] = x;
            vertices[(x + z * m_data.size) * 3 + 1] = getHeight(x, z);
            vertices[(x + z * m_data.size) * 3 + 2] = z;
        }
    }

    // create index data
    unsigned int *indices = new unsigned int[m_data.size * m_data.size * 6];
    for (int z = 0; z < m_data.size - 1; z++) {
        for (int x = 0; x < m_data.size - 1; x++) {
            indices[(x + z * m_data.size) * 6 + 0] = x + z * m_data.size;
            indices[(x + z * m_data.size) * 6 + 1] = x + (z + 1) * m_data.size;
            indices[(x + z * m_data.size) * 6 + 2] = x + 1 + z * m_data.size;
            indices[(x + z * m_data.size) * 6 + 3] = x + 1 + z * m_data.size;
            indices[(x + z * m_data.size) * 6 + 4] = x + (z + 1) * m_data.size;
            indices[(x + z * m_data.size) * 6 + 5] = x + 1 + (z + 1) * m_data.size;
        }
    }

    // upload vertex data
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_data.size * m_data.size * 3 * sizeof(float), vertices);

    // upload index data
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_data.size * m_data.size * 6 * sizeof(unsigned int), indices);

    // delete temporary data
    delete[] vertices;
    delete[] indices;

    // setup vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Terrain::render() {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_data.size * m_data.size * 6, GL_UNSIGNED_INT, 0);
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

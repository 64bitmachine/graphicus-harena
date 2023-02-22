#include "camera.h"

void Camera::update_camera_vectors() {
    // new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // new right vector
    m_right = glm::normalize(glm::cross(m_front, m_world_up));

    // new up vector
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

Camera::Camera(glm::vec3 position) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
           m_movement_speed(SPEED), m_mouse_sensitivity(SENSITIVITY), m_zoom(ZOOM) {
    m_position = position;
    m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_yaw = YAW;
    m_pitch = PITCH;
    m_zoom = ZOOM;
    update_camera_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, 
float up_y, float up_z, float yaw, float pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
           m_movement_speed(SPEED), m_mouse_sensitivity(SENSITIVITY), m_zoom(ZOOM) {
    m_position = glm::vec3(pos_x, pos_y, pos_z);
    m_world_up = glm::vec3(up_x, up_y, up_z);
    m_yaw = yaw;
    m_pitch = pitch;
    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::process_keyboard(Cam_Movement direction, float delta_time) {
    float velocity = m_movement_speed * delta_time;
    if (direction == FORWARD) {
        m_position += m_front * velocity;
    }
    if (direction == BACKWARD) {
        m_position -= m_front * velocity;
    }
    if (direction == LEFT) {
        m_position -= m_right * velocity;
    }
    if (direction == RIGHT) {
        m_position += m_right * velocity;
    }
}

void Camera::process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch) {
    x_offset *= m_mouse_sensitivity;
    y_offset *= m_mouse_sensitivity;

    m_yaw += x_offset;
    m_pitch += y_offset;

    if (constrain_pitch) {
        if (m_pitch > 89.0f) {
            m_pitch = 89.0f;
        }
        if (m_pitch < -89.0f) {
            m_pitch = -89.0f;
        }
    }

    // update front, right and up vectors using the updated euler angles
    update_camera_vectors();
}

void Camera::process_mouse_scroll(float y_offset) {
    if (m_zoom >= 1.0f && m_zoom <= 45.0f) {
        m_zoom -= y_offset;
    }
    if (m_zoom <= 1.0f) {
        m_zoom = 1.0f;
    }
    if (m_zoom >= 45.0f) {
        m_zoom = 45.0f;
    }
}

float Camera::get_zoom() const { return m_zoom; }

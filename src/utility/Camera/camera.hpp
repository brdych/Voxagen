#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

#include <worldvariables.hpp>
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  8.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;
const float S_MUL       =  5.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    bool SpeedToggle;
    glm::vec3 ePos;
    float ArcDistance = 10.0f;


    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        SpeedToggle = false;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        if(!WorldVariables::EDIT_MODE)
        {
            return glm::lookAt(Position, Position + Front, Up);
        }
        else
        {
            ePos = glm::vec3(WorldVariables::EDIT_POS.x+0.5,WorldVariables::EDIT_POS.y+0.5,WorldVariables::EDIT_POS.z+0.5);


            glm::mat4 ViewTranslate = glm::mat4(1.0f);

            glm::mat4 RY = glm::rotate(ViewTranslate,glm::radians(Yaw),glm::vec3(0.0f,1.0f,0.0f));//yaw
            glm::mat4 RX = glm::rotate(RY,glm::radians(Pitch),glm::vec3(1.0f,0.0f,0.0f));//pitch
            glm::mat4 R = glm::rotate(RX,0.0f,glm::vec3(0.0f,0.0f,1.0f));//no roll, currently
            glm::vec3 T = glm::vec3(0,0,ArcDistance);
            T = glm::vec3(R*glm::vec4(T,0.0f));
            Position = ePos + T;
            return glm::lookAt(Position, ePos, Up);
        }
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        if(!WorldVariables::EDIT_MODE)
        {
            float velocity = MovementSpeed * deltaTime;
            if (direction == FORWARD)
                Position += Front * velocity * (SpeedToggle ? S_MUL : 1);
            if (direction == BACKWARD)
                Position -= Front * velocity * (SpeedToggle ? S_MUL : 1);
            if (direction == LEFT)
                Position -= Right * velocity * (SpeedToggle ? S_MUL : 1);
            if (direction == RIGHT)
                Position += Right * velocity * (SpeedToggle ? S_MUL : 1);
        }
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        if(WorldVariables::EDIT_MODE)
        {
            MouseSensitivity = 0.1f;
        }
        else {
            MouseSensitivity = 0.1f;
        }

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        if(Yaw > 360)
        {
            Yaw -= 360;
        }
        if(Yaw < 0)
        {
            Yaw += 360;

        }

        if(abs(Front.x) > abs(Front.z))
        {
            if(Front.x > 0)
                WorldVariables::CARDINAL_DIRECTION = 1; //North Pos X
            else
                WorldVariables::CARDINAL_DIRECTION = 3; //South Neg X
        }
        else
        {
            if(Front.z > 0)
                WorldVariables::CARDINAL_DIRECTION = 2; //East Pos Z
            else
                WorldVariables::CARDINAL_DIRECTION = 4; //West Neg Z
        }

        Pitch += yoffset;
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float xoffset, float yoffset)
    {
        ArcDistance -= yoffset;
        if(ArcDistance<=1)
            ArcDistance = 1;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        if(!WorldVariables::EDIT_MODE)
        {
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
        }
        else
        {
            Front = glm::normalize(ePos-Position);
        }
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif

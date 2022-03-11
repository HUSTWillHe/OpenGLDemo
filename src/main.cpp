#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "learnopengl/shader.h"
#include "utils/genGaussWeights.h"
#include "utils/setRadiusInFs.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv)
{
	int radius = 0;
	if (argc == 2) {
		std::string str(argv[1]);
		radius = std::stoi(str);
	} else {
		radius = 3;
	}
	std::vector<float> gaussWeight = genGaussWeight(radius);
	std::cout<<"====================================="<<std::endl;
	std::cout<<"gauss weight"<<std::endl;
	for (auto i = 0; i < radius + 1; i++) {
		for (auto j = 0; j < radius + 1; j++) {
			std::cout<<gaussWeight[i * (radius + 1) + j]<<" ";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
	setRadius(radius, "../src/texture.backup.fs", "../src/texture.fs");  // 这里并没有实时修改过来
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("../src/texture.vs", "../src/texture.fs"); 

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width = 800, height = 800, nrChannels = 3;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    std::string image_path = "goods.jpg";
    unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);  

	unsigned int maskTexture;
	glGenTextures(1, &maskTexture);
	glBindTexture(GL_TEXTURE_2D, maskTexture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
	image_path = "mask.jpg";
	nrChannels = 1;
	data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data); //todo 试一下GL_RED如果不行就gl_alpha, todo 怎么加载第二张图片
        glGenerateMipmap(GL_TEXTURE_2D);
	} else {
        std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// fbo
	unsigned int fboTexture;
	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	// set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int renderBufferId;
	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); 

	unsigned int frameBufferId;
	glGenFramebuffers(1, &frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	ourShader.use();
	ourShader.setInt("ourTexture0", 0);
	ourShader.setInt("ourTexture1", 1);
	ourShader.setInt("width", width);
	ourShader.setInt("height", height);
	glUniform1fv(glGetUniformLocation(ourShader.ID, "gaussWeight"), (radius + 1) * (radius + 1), gaussWeight.data());

    // render loop
    // -----------
	int cnt = 0;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

		if (cnt == 10) {
			std::cout<<"now write file"<<std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindTexture(GL_TEXTURE_2D, texture);
			ourShader.use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glBindTexture(GL_TEXTURE_2D, fboTexture);
			std::vector<char> buf(width * height * 3);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)buf.data());
			std::fstream f("./image.rgb", std::ios::out | std::ios::binary);
			f.write(buf.data(), buf.size());
			f.close();
			cnt ++;
			glfwSwapBuffers(window);
			glfwPollEvents();
			continue;
		}

        // render
        // ------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        ourShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, maskTexture);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		cnt ++;
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
	glDeleteFramebuffers(1, &frameBufferId);
	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &maskTexture);
	glDeleteTextures(1, &fboTexture);
	glDeleteRenderbuffers(1, &renderBufferId);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// Demonstrates the interaction between OpenCL and OpenGL
// OpenCL generates a gradient texture that stays on the GPU side
// OpenGL then uses the texture to draw the resulting image to the screen

#include <stdio.h>
#include <stdlib.h>

// Platforms differ in header location and context initialization
#ifdef WIN32 // Windows
    #include <GL/freeglut.h>
    #include <GL/freeglut_ext.h>
    #include <CL/cl.h>
    #include <CL/cl_gl.h>
    #define CL_CONTEXT_PROP { CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0}
#else
    #ifdef __APPLE__ // OSX
        #include <GLUT/glut.h>
        #include <OpenGL/OpenGL.h>
        #include <OpenGL/CGLDevice.h>
        #include <OpenCL/opencl.h>
        #include <OpenCL/cl_gl_ext.h>
        #define CL_CONTEXT_PROP { CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)CGLGetShareGroup(CGLGetCurrentContext()), CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0}
    #else // Linux/Unix X11
        #include <GL/freeglut.h>
        #include <GL/freeglut_ext.h>
        #include <CL/cl.h>
        #include <CL/cl_gl.h>
        #include <GL/glx.h>
        #define CL_CONTEXT_PROP { CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(), CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(), CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0}
    #endif
#endif

#define TEX_SIZE 512

GLuint texture;

cl_command_queue command_queue;
cl_kernel kernel;
cl_program program;
cl_mem mem_obj;
cl_context context;
cl_int ret;

void initCL() {
    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    
    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;
    
    fp = fopen("gradient.cl", "r");
    if (!fp) {
        
        fprintf(stderr, "Failed to load kernel.\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0L, SEEK_END);
    source_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    source_str = (char*)malloc(source_size);
    fread( source_str, 1, source_size, fp);
    fclose( fp );
    
    // Get available platforms and devices
    clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    
    // Set platform specific OpenCL contect properties
    cl_context_properties prop[] = CL_CONTEXT_PROP;
    
    // Create an OpenCL context
    context = clCreateContext( prop, 1, &device_id, NULL, NULL, &ret);
    if (!context) {
        printf("Failed creating OpenCL context.\n");
        exit(EXIT_FAILURE);
    }
    
    // Create a command queue
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    if (!command_queue) {
        printf("Failed creating command queue.\n");
        exit(EXIT_FAILURE);    
    }
    
    // Get OpenGL texture memory object
    mem_obj = clCreateFromGLTexture2D(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0,texture, &ret);
    if (!mem_obj) {
        printf("Failed creating memory object from OpenGL texture.\n");
        exit(EXIT_FAILURE);
    }
    
    // Create a program from the kernel source
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    if (!program) {
        printf("Failed creating OpenCL program.\n");
        exit(EXIT_FAILURE);
    }
    
    // Build the program
    clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    
    // Create the OpenCL kernel
    kernel = clCreateKernel(program, "gradient", &ret);
    if (!kernel) {
        printf("Failed creating kernel.\n");
        exit(EXIT_FAILURE);
    }
    
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&mem_obj);
    if (ret != CL_SUCCESS) {
        printf("Failed setting up kernel arguments.\n");
        exit(EXIT_FAILURE);
    }
}

void GenerateImageCL() {
    //we use 2 dimensions (x,y)
    size_t global_work_size[2];
    global_work_size[0] = TEX_SIZE;
    global_work_size[1] = TEX_SIZE;
    
    clEnqueueAcquireGLObjects(command_queue, 1, &mem_obj, 0,0,0);
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        printf("Failed to queue kernel execution.\n");
        exit(EXIT_FAILURE);
    }
    clEnqueueReleaseGLObjects(command_queue, 1, &mem_obj, 0,0,0);
    clFlush(command_queue);
}

void cleanCL()
{
    // Clean up
    clFlush(command_queue);
    clFinish(command_queue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseMemObject(mem_obj);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
}

// Initialize OpenGL LoadTexstate
void initGL()
{
    // Texture setup
    glEnable(GL_TEXTURE_2D);
    glGenTextures( 1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    // Other States
    glClearColor(0,0,0,0);
    gluOrtho2D(-1,1,-1,1);
    glLoadIdentity();
    glColor3f(1,1,1);
    
    // Make Texture
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

// Generate and display the image.
void display() {
    // Call user image generation
    GenerateImageCL();
    // Set texture
    glBindTexture(GL_TEXTURE_2D, texture);
    // Clear screen buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // Render a quad
    glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex2f(1,-1);
    glTexCoord2f(1,1); glVertex2f(1,1);
    glTexCoord2f(0,1); glVertex2f(-1,1);
    glTexCoord2f(0,0); glVertex2f(-1,-1);
    glEnd();
    // Display result
    glFlush();
    glutPostRedisplay();
    glutSwapBuffers();
}

// Main entry function
int main(int argc, char ** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitWindowSize(TEX_SIZE, TEX_SIZE);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("OpenGL Window");
    
    // Set up OpenGL/OpenCL contexts
    initGL();
    initCL();
    
    // Run the control loop
    glutDisplayFunc(display);
    glutMainLoop();
    
    // Clean up
    cleanCL();
    
    return EXIT_SUCCESS;
}

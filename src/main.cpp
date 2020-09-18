#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>

#define DRAW_CUBIC_BEZIER 1 // Use to switch Linear and Cubic bezier curves
#define SAMPLES_PER_BEZIER 10 //Sample each Bezier curve as N=10 segments and draw as connected lines

// GLobal variables
std::vector<float> controlPoints;
std::vector<float> curvePoints;
std::vector<float> linearBezier;
std::vector<float> cubicBezier;
int width = 640, height = 640; 
bool controlPointsUpdated = false;

void calculatePiecewiseLinearBezier()
{
    // Since linearBezier is just a polyline, we can just copy the control points and plot.
    // However to show how a piecewise parametric curve needs to be plotted, we sample t and 
    // evaluate all linear bezier curves.
    // linearBezier.assign(controlPoints.begin(), controlPoints.end());

    linearBezier.clear();
    int sz  = controlPoints.size(); // Contains 3 points/vertex. Ignore Z
    float x[2], y[2];
    float delta_t = 1.0/(SAMPLES_PER_BEZIER - 1.0);
    float t;
    for(int i=0; i<(sz-3); i+=3) {
        x[0] = controlPoints[i];
        y[0] = controlPoints[i+1];
        x[1] = controlPoints[i+3];
        y[1] = controlPoints[i+4];
        linearBezier.push_back(x[0]);
        linearBezier.push_back(y[0]);
        linearBezier.push_back(0.0);
        t = 0.0;
        for (float j=1; j<(SAMPLES_PER_BEZIER-1); j++)
        {
        t += delta_t;
        linearBezier.push_back(x[0] + t*(x[1] - x[0]));
        linearBezier.push_back(y[0] + t*(y[1] - y[0]));
        linearBezier.push_back(0.0);
        }
        // No need to add the last point for this segment, since it will be added as first point in next.
    }
    // However, add last point of entire piecewise curve here (i.e, the last control point)
    linearBezier.push_back(x[1]);
    linearBezier.push_back(y[1]);
    linearBezier.push_back(0.0);
}

void calculatePiecewiseCubicBezier()
{
    // TODO 
    cubicBezier.clear();
    curvePoints.clear();
    int sz  = controlPoints.size(); // Contains 3 points/vertex. Ignore Z
    float x[4], y[4];
    float delta_t = 1.0/(SAMPLES_PER_BEZIER - 1.0);
    float t;
    {
    int i=0;
    while(i<sz-3){
        if(i==0){
            curvePoints.push_back(controlPoints[i]);
            curvePoints.push_back(controlPoints[i+1]);
            curvePoints.push_back(0.0);
            curvePoints.push_back(controlPoints[i]+0.1);
            curvePoints.push_back(controlPoints[i+1]+0.2);
            curvePoints.push_back(0.0);
            i+=3;
        }else{
        curvePoints.push_back(controlPoints[i] - 0.1*cos(45.0));
        curvePoints.push_back(controlPoints[i+1] - 0.1*sin(45.0));
        curvePoints.push_back(0.0);
        curvePoints.push_back(controlPoints[i]);
        curvePoints.push_back(controlPoints[i+1]);
        curvePoints.push_back(0.0);
        curvePoints.push_back(controlPoints[i] + 0.1*cos(45.0));
        curvePoints.push_back(controlPoints[i+1] + 0.1*sin(45.0));
        curvePoints.push_back(0.0);
        i+=3;
            }
    }

    } 
    for(int j=0;j<curvePoints.size();j++){
        std::cout << curvePoints[j]<< " ";
    }
    // if(j==1){
    //     std::cout << "Hello";
    //     prevX[0] = controlPoints[0] + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(controlPoints[3]-controlPoints[0])));
    //     prevY[0] = controlPoints[1] + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(controlPoints[4]-controlPoints[1])));
    //     prevX[1] = controlPoints[0] + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(controlPoints[3]-controlPoints[0])));
    //     prevY[1] = controlPoints[1] + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(controlPoints[4]-controlPoints[1])));
    // }else if(j>1){
    //     prevX[0] = prevX[1];
    //     prevY[0] = prevY[1];
    //     prevX[1] = prevX[0] + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(controlPoints[j-1]-prevX[0])));
    //     prevY[1] = prevY[0] + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(controlPoints[j]-prevY[0])));
    // }

    for(int i=0; i<sz; i+=3) {
        std::cout << curvePoints.size()<<"hello";
        x[0] = curvePoints[i];
        y[0] = curvePoints[i+1];

        x[1] = curvePoints[i+3];
        y[1] = curvePoints[i+4];

        x[2] = curvePoints[i+6];
        y[2] = curvePoints[i+7];

        x[3] = curvePoints[i+9];
        y[3] = curvePoints[i+10];

        cubicBezier.push_back(x[0]);
        cubicBezier.push_back(y[0]);
        cubicBezier.push_back(0.0);
        t = 0.0;
        for (float j=1; j<(SAMPLES_PER_BEZIER-1); j++)
        {
            t += delta_t;
            cubicBezier.push_back((1-t)*(1-t)*(1-t)*x[0]+3*(1-t)*(1-t)*t*x[1]+3*(1-t)*t*t*x[2]+t*t*t*x[3]);
            cubicBezier.push_back((1-t)*(1-t)*(1-t)*y[0]+3*(1-t)*(1-t)*t*y[1]+3*(1-t)*t*t*y[2]+t*t*t*y[3]);
            cubicBezier.push_back(0.0);
        }
        // No need to add the last point for this segment, since it will be added as first point in next.
    }
    // However, add last point of entire piecewise curve here (i.e, the last control point)
    cubicBezier.push_back(x[3]);
    cubicBezier.push_back(y[3]);
    cubicBezier.push_back(0.0);
}

int main(int, char* argv[])
{
    GLFWwindow* window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
	glUseProgram(shaderProgram);

    // Create VBOs, VAOs
    unsigned int VBO_controlPoints, VBO_linearBezier, VBO_cubicBezier;
    unsigned int VAO_controlPoints, VAO_linearBezier, VAO_cubicBezier;
    glGenBuffers(1, &VBO_controlPoints);
    glGenVertexArrays(1, &VAO_controlPoints);
    glGenBuffers(1, &VBO_linearBezier);
    glGenVertexArrays(1, &VAO_linearBezier);
    //TODO:
    glGenBuffers(1, &VBO_cubicBezier);
    glGenVertexArrays(1, &VAO_cubicBezier);

    int button_status = 0;

    //Display loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering
        showOptionsDialog(controlPoints, io, curvePoints);
        ImGui::Render();

        // Add a new point on mouse click
        float x,y ;
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        if(io.MouseClicked[0] && !ImGui::IsAnyItemActive()){
            x = io.MousePos.x;
            y = io.MousePos.y;
            addPoints(controlPoints, x, y, width, height);
            // if(controlPoints.size()==1){
            //     curvePoints.push_back(x);
            //     curvePoints.push_back(y);
            //     curvePoints.push_back(0.0);

            //     curvePoints.push_back(x+0.1);
            //     curvePoints.push_back(y+0.1);
            //     curvePoints.push_back(0.0);
            // }else{
            //     curvePoints.push_back(x - 0.1*cos(45.0));
            //     curvePoints.push_back(y - 0.1*sin(45.0));
            //     curvePoints.push_back(0.0);

            //     curvePoints.push_back(x);
            //     curvePoints.push_back(y);
            //     curvePoints.push_back(0.0);

            //     curvePoints.push_back(x + 0.1*cos(45.0));
            //     curvePoints.push_back(y + 0.1*sin(45.0));
            //     curvePoints.push_back(0.0);
            //     // addPoints(curvePoints, x - 0.1*cos(45.0), y - 0.1*sin(45.0) , width, height);
            //     // addPoints(curvePoints, x, y, width, height);
            //     // addPoints(curvePoints, x + 0.1*cos(45.0), y + 0.1*sin(45.0), width, height);
            // }
            controlPointsUpdated = true;
         }

        if(controlPointsUpdated) {
            // Update VAO/VBO for control points (since we added a new point)
            glBindVertexArray(VAO_controlPoints);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_controlPoints);
            glBufferData(GL_ARRAY_BUFFER, controlPoints.size()*sizeof(GLfloat), &controlPoints[0], GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0); //Enable first attribute buffer (vertices)

            // Update VAO/VBO for piecewise linear Bezier curve (since we added a new point)
            calculatePiecewiseLinearBezier();
            glBindVertexArray(VAO_linearBezier);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_linearBezier);
            glBufferData(GL_ARRAY_BUFFER, linearBezier.size()*sizeof(GLfloat), &linearBezier[0], GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0); //Enable first attribute buffer (vertices)

            // Update VAO/VBO for piecewise cubic Bezier curve
            // TODO:
            calculatePiecewiseCubicBezier();
            glBindVertexArray(VAO_cubicBezier);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_cubicBezier);
            glBufferData(GL_ARRAY_BUFFER, cubicBezier.size()*sizeof(GLfloat), &cubicBezier[0], GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);


            controlPointsUpdated = false; // Finish all VAO/VBO updates before setting this to false.
        }

        glUseProgram(shaderProgram);

        // Draw control points
        glBindVertexArray(VAO_controlPoints);
		glDrawArrays(GL_POINTS, 0, controlPoints.size()/3); // Draw points

#if DRAW_CUBIC_BEZIER
        // TODO:
        glBindVertexArray(VAO_cubicBezier);
        glDrawArrays(GL_LINE_STRIP, 0 , cubicBezier.size()/3);
        
#else
        // Draw linear Bezier
        glBindVertexArray(VAO_linearBezier);
        glDrawArrays(GL_LINE_STRIP, 0, linearBezier.size()/3); //Draw lines
#endif

        glUseProgram(0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

    }

    // Delete VBO buffers
    glDeleteBuffers(1, &VBO_controlPoints);
    glDeleteBuffers(1, &VBO_linearBezier);
    //TODO:
    glDeleteBuffers(1,&VBO_cubicBezier);
    // Cleanup
    cleanup(window);
    return 0;
}

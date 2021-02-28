#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>


const int W = 800; 
const int H = 600; 

// So we can save screenshots without the GUI 
bool canDrawGUI = true;
bool canDrawAbout = false;

// Shader uniforms 
float timeUniform = 0.0f;
int iterationUniform = 64;
float mod1Uniform = 5.0f;
float mod2Uniform = 0.755f;
float panModUniform = 0.0f;

float value1 = 1.0f;//1.5;
float value2 = 1.0f;//0.1;

float z1 = 2.0f;
float z2 = 1.0f; 


float bVal = 20.0f;


int main()
{   
    sf::RenderWindow window(sf::VideoMode(W, H), "Glowing Julia");
    ImGui::SFML::Init(window);

    // Setup the icon 
    sf::Image icon;
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Setup the main tex 
    sf::Texture tex;
    tex.create(W, H);
    sf::Sprite spr(tex); 

    // Shader stuff 
    sf::Shader shader; 
    shader.loadFromFile("frag.glsl", sf::Shader::Fragment);
    if (!shader.isAvailable())
    {
        std::cout << "The shader is not avaliable.\n";
    }

    // Clocks 
    sf::Clock guiClock;
    sf::Clock shaderClock;
    shaderClock.restart();

    shader.setUniform("resolution", sf::Vector2f(W, H));

    // Style (made with the ImGui Style Editor 
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 0.0f;
    style.ScrollbarRounding = 0.0f;
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.31f, 0.87f, 0.94f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.06f, 0.73f, 0.10f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.06f, 0.73f, 0.10f, 0.40f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.52f, 0.28f, 0.81f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.52f, 0.28f, 0.81f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.85f, 0.17f, 0.29f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.17f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.85f, 0.17f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.85f, 0.17f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.17f, 0.31f, 1.00f);


    // Main loop 
    while (window.isOpen())
    {
        // Poll for events 
        sf::Event e;
        while (window.pollEvent(e))
        {
            ImGui::SFML::ProcessEvent(e);
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }

            // More stuff to go here 
            // Reset the resolution uniform when we maximize 
            if (e.type == sf::Event::Resized)
            {
                shader.setUniform("resolution", sf::Vector2f(e.size.width, e.size.height));
            }
        }


        // Update the gui
        ImGui::SFML::Update(window, guiClock.restart());

        // Draw the GUI
        ImGui::Begin("Glowing Julia Controls");
        ImGui::SliderInt("Iterations", &iterationUniform, 0, 600);
        ImGui::SliderFloat("Mod 1", &mod1Uniform, 0.0, 50.0);
        ImGui::SliderFloat("Mod 2", &mod2Uniform, 0.65, 0.9);
        ImGui::SliderFloat("Value 1", &value1, 0.95, 1.05);
        ImGui::SliderFloat("Value 2", &value2, -20.0, 20.0);
        ImGui::Separator();
        ImGui::SliderFloat("Break value", &bVal, 0.0, 30.0);
        ImGui::SliderFloat("Zoom 1", &z1, 0.01, 2.0);
        ImGui::SliderFloat("Zoom 2", &z2, 0.01, 1.0);
        if (ImGui::Button("Save Screenshot"))
        {
            canDrawGUI = false;
        }
        if (ImGui::Button("About"))
        {
            canDrawAbout = true;
        }
        ImGui::End();

        if (canDrawAbout)
        {
            ImGui::Begin("About");
            ImGui::Text("Glowing Julia v 0.9");
            ImGui::Separator();
            ImGui::Text("By Jessica Leyba 2021");
            ImGui::Text("s0ftwave.com");
            if (ImGui::Button("Close"))
            {
                canDrawAbout = false;
            }
            ImGui::End();
        }

        // Update uniforms
        timeUniform = shaderClock.getElapsedTime().asSeconds() * 2.0;

        // Set uniforms 
        
        shader.setUniform("time", timeUniform);
        shader.setUniform("iterations", iterationUniform);
        shader.setUniform("mod1", mod1Uniform);
        shader.setUniform("mod2", mod2Uniform);
        shader.setUniform("val1", value1);
        shader.setUniform("val2", value2);
        shader.setUniform("brkVal", bVal);
        shader.setUniform("zoom1", z1);
        shader.setUniform("zoom2", z2);

        // Actually draw to the screen 
        window.clear();
        window.draw(spr, &shader);
        if (canDrawGUI)
        {
            ImGui::SFML::Render(window);
        }
        else
        {
            sf::Texture screenTex;
            time_t rawtime;
            struct tm* timeinfo;
            char filename[80];
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(filename, 80, "screen-%Y-%m-%d-%H-%M-%S.png", timeinfo);

            screenTex.create(window.getSize().x, window.getSize().y);
            screenTex.update(window);
            if (screenTex.copyToImage().saveToFile(filename))
            {
                std::cout << "Screenshot saved as " << filename << std::endl;
            }
            canDrawGUI = true;
        }
        window.display();
    }

    return 0;
}
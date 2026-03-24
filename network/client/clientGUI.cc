/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientGUI.cc                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:31:58 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/24 09:40:18 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hh"
#include "imgui.h"
#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <stdio.h>
#include "imgui_stdlib.h"
#include <SDL3/SDL.h>

struct LoginState {
    std::string username;
    std::string password;
    bool request = false;
};

void ButtonCenteredOnLine(std::string label, float alignment = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label.data()).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    ImGui::Text(label.c_str(), label.size());
}

void interacte(std::string name, lpp::client &client, bool &done){
	static int code;
	static std::string str;
	static bool send = false;
	static std::string reply{};
	ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoCollapse;

    ImGui::Begin(name.c_str(), nullptr, window_flags);
	ImGui::Text("code      action");
	ImGui::PushItemWidth(50);
	ImGui::InputInt("##left", &code, 0, 0, ImGuiInputTextFlags_CharsDecimal);
	ImGui::PushItemWidth(150);
	ImGui::SameLine();
	ImGui::Text("|");
	ImGui::SameLine();
	if (ImGui::InputText("##right", &str, ImGuiInputTextFlags_EnterReturnsTrue))
		send = true;
	ImGui::SameLine();
	if (ImGui::Button("send")){
		send = true;
	}
	if (send){
		if (!code && !str.empty())
			reply = client.send(str);
		else{
			std::string tmp(std::to_string(code) + "|" + str);
			reply = client.send(tmp.c_str(), tmp.size());
		}
		client.getLogger().log(lpp::INFO, "send: " + str);
		if (reply.find('|') != std::string::npos)
			reply = client.received(reply);
		if (str == "quit")
			done = true;
		str.clear();
		send = false;
		ImGui::SetKeyboardFocusHere(-1);
		code = 0;
	}

	ImGui::SeparatorText("answer");
	ImGui::Text(reply.c_str(), reply.size());
	ImGui::End();
}

void login(LoginState& state, lpp::client &client)
{
    ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Always);
    ImGui::SetNextWindowPos(
        ImGui::GetMainViewport()->GetCenter(),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );

	static std::string reply;
	if (reply != "connected"){
		ImGui::Begin("Login", nullptr,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse);

		bool send = false;

		if (ImGui::InputText("Username", &state.username,
							 ImGuiInputTextFlags_EnterReturnsTrue))
			ImGui::SetKeyboardFocusHere();

		if (ImGui::InputText("Password", &state.password,
							 ImGuiInputTextFlags_Password |
							 ImGuiInputTextFlags_EnterReturnsTrue))
			send = true;

		if (ImGui::Button("Login", ImVec2(-1, 0)))
			send = true;

		if (send && !state.username.empty() && !state.password.empty()){
			lpp::message connect(1);
			connect << "username=" << state.username << " " << "password=" << state.password;
			reply = client.send(connect);
			state.password.clear();
			std::erase(reply, '\n');
		} else if (!reply.empty() && state.request == false){
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("   Wrong password, connection refused");
			ImGui::PopStyleColor();
		}
		ImGui::End();
	} else if (reply == "connected"){
		volatile static int i = 0;
		i += 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		ImGui::Begin("logged", nullptr,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse);
		
		ImGui::Text("");
		ImGui::Text("");
		ButtonCenteredOnLine("Connected !");
		if (i == 15)
			state.request = true;
		ImGui::End();
	}
}

int main(void){

	lpp::client client;
	client.getLogger().setIsStdout(true);
	client.getLogger().setPrintFormat(true);

	client.defineAction(2, [](const lpp::message& msg){
        int doubledValue = 0;
		msg >> doubledValue;
        lpp::cout << "Received a doubled value: " << doubledValue << std::endl;
    });

	client.defineAction(10, [&client](const lpp::message& msg){
		std::string str;
        msg >> str;
		lpp::cout << str << std::endl;
		if (str == "pong")
			client.getLogger().log(lpp::INFO, "server work!");
	});

	try{
		client.connect("127.0.0.1", 4242);
	} catch(std::exception &e){
		lpp::cout << "client error: " << e.what() << std::endl;
		return(-1);
	}


    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("clientGUI", (int)(500 * main_scale), (int)(300 * main_scale), window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr)
    {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(5.00f, 2.00f);
	style.CellPadding = ImVec2(6.00f, 6.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 11;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;
	style.TabBorderSize = 1;
	style.WindowRounding = 7;
	style.ChildRounding = 4;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ScrollbarRounding = 9;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
	style.FontSizeBase = 25;

	auto colors = style.Colors;
	colors[ImGuiCol_Text] = ImVec4(0.92f, 0.93f, 0.94f, 1.00f);                  // Light grey text for readability
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.52f, 0.54f, 1.00f);          // Subtle grey for disabled text
	colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);              // Dark background with a hint of blue
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);               // Slightly lighter for child elements
	colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);               // Popup background
	colors[ImGuiCol_Border] = ImVec4(0.28f, 0.29f, 0.30f, 0.60f);                // Soft border color
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);          // No border shadow
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);               // Frame background
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.24f, 0.26f, 1.00f);        // Frame hover effect
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.26f, 0.28f, 1.00f);         // Active frame background
	colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);               // Title background
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);         // Active title background
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);      // Collapsed title background
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);             // Menu bar background
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);           // Scrollbar background
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.26f, 0.28f, 1.00f);         // Dark accent for scrollbar grab
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.30f, 0.32f, 1.00f);  // Scrollbar grab hover
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.32f, 0.34f, 0.36f, 1.00f);   // Scrollbar grab active
	colors[ImGuiCol_CheckMark] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);             // Dark blue checkmark
	colors[ImGuiCol_SliderGrab] = ImVec4(0.36f, 0.46f, 0.56f, 1.00f);            // Dark blue slider grab
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.50f, 0.60f, 1.00f);      // Active slider grab
	colors[ImGuiCol_Button] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);                // Dark blue button
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.38f, 0.48f, 1.00f);         // Button hover effect
	colors[ImGuiCol_ButtonActive] = ImVec4(0.32f, 0.42f, 0.52f, 1.00f);          // Active button
	colors[ImGuiCol_Header] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);                // Header color similar to button
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.38f, 0.48f, 1.00f);         // Header hover effect
	colors[ImGuiCol_HeaderActive] = ImVec4(0.32f, 0.42f, 0.52f, 1.00f);          // Active header
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);             // Separator color
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);      // Hover effect for separator
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);       // Active separator
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.46f, 0.56f, 1.00f);            // Resize grip
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.50f, 0.60f, 1.00f);     // Hover effect for resize grip
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.44f, 0.54f, 0.64f, 1.00f);      // Active resize grip
	colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);                   // Inactive tab
	colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.38f, 0.48f, 1.00f);            // Hover effect for tab
	colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);             // Active tab color
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);          // Unfocused tab
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);    // Active but unfocused tab
	colors[ImGuiCol_PlotLines] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);             // Plot lines
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);      // Hover effect for plot lines
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.36f, 0.46f, 0.56f, 1.00f);         // Histogram color
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.40f, 0.50f, 0.60f, 1.00f);  // Hover effect for histogram
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);         // Table header background
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);     // Strong border for tables
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.24f, 0.25f, 0.26f, 1.00f);      // Light border for tables
	colors[ImGuiCol_TableRowBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);            // Table row background
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.22f, 0.24f, 0.26f, 1.00f);         // Alternate row background
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.34f, 0.44f, 0.35f);        // Selected text background
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.46f, 0.56f, 0.66f, 0.90f);        // Drag and drop target
	colors[ImGuiCol_NavHighlight] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);          // Navigation highlight
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f); // Windowing highlight
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);     // Dim background for windowing
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);      // Dim background for modal windows

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;

    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        int winWidth, winHeight;
        SDL_GetWindowSize(window, &winWidth, &winHeight);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(winWidth), static_cast<float>(winHeight)));

		static LoginState	log;

		if (!log.request)
			login(log, client);
		else
			interacte(log.username, client, done);

        ImGui::Render();
        SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

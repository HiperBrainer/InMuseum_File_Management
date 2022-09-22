// Dear ImGui: standalone example application for DirectX 11
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include "ABBB.h"
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION


using namespace std;

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
static ID3D11ShaderResourceView* my_texture = NULL;


// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Struct for new adding arts
typedef struct newArt
{
    int rrn;
    char name[20];
    char block[20];
    char autor[20];
}newArt;

int convertToId(char vector[])
{
    int id = 0;
    for (int i = 0; i < 20; i++)
    {
        id += static_cast<int>(vector[i]);
    }
    return id;
}

void loadFromFile(ABBB* arvore, int& rrn)
{
    ifstream file;

    file.open("museumFile", ios::in | ios::binary);
    if (!file) {
        cout << "Error opening the file\n";
        return;
    }

    newArt art;

    while (file.read((char*)&art, sizeof(newArt)))
    {
        arvore->root = arvore->insert(arvore->root, convertToId(art.name), art.rrn);
        rrn++;
    }

    file.close();
}

void insertFIle(ABBB* arvore, item data)
{
    ofstream file;

    file.open("file", ios::out | ios::binary);

    if (!file) {
        cout << "Error opening the file\n";
        return;
    }
}

void clearChar(char vector[])
{
    for (int i = 0; i < 20; i++)
    {
        vector[i] = *"";
    }
}

void stringToString(char vector1[], char vector2[])
{
    for (int i = 0; i < 20; i++)
    {
        vector1[i] = vector2[i];
    }
}

void toLower(char vector[])
{
    for (int i = 0; i < 20; i++)
    {
        vector[i] = tolower(vector[i]);
    }
}

void writeToFile(newArt art)
{

    ofstream arquivo;
    arquivo.open("museumFile", ios::app | ios::binary);

    if (!arquivo) {
        cout << "Erro ao abrir o arquivo\n";
        return;
    }

    arquivo.write((char*)&art, sizeof(struct newArt));


    cout << "\nFechando arquivo...\n\n\n";

    arquivo.close();
}

void readFromFIle()
{
    ifstream arquivoin;
    arquivoin.open("museumFile", ios::in | ios::binary);
    if (!arquivoin) {
        cout << "Erro ao abrir o arquivo\n";
        return;
    }

    newArt art;

    while (arquivoin.read((char*)&art, sizeof(struct newArt))) {
        cout << "Nome: " << art.name << " Bloco: " << art.block << " Autor: " << art.autor << endl;
        cout << "RRN: " << art.rrn << endl;
    }

    arquivoin.close();

}

newArt returnItem(int rrn)
{
    newArt art;
    rrn = rrn * sizeof(newArt);

    ifstream arquivo;

    arquivo.open("museumFile", ios::in | ios::binary);

    if (arquivo.seekg(rrn)) {
        cout << "Register found" << endl;
        if (arquivo.read((char*)&art, sizeof(newArt)))
            return art;
    }
    else
        cout << "Register not found" << endl;

    arquivo.close();
}

bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    return true;
}

// Main code
int main(int, char**)
{
    int rrn = 0;
    ABBB* arvore = new ABBB();
    loadFromFile(arvore, rrn);
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();

    int my_image_width = 0;
    int my_image_height = 0;
    bool ret = LoadTextureFromFile("C:/Users/pedro/Downloads/flor.jpg", &my_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX11 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

       // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    char inputName[20] = "";
    char inputUsername[20] = "";
    char inputPassword[20] = "";
    char username[20] = "admin";
    char password[20] = "admin";

    bool logged = false;
    bool searched = false;
    int numero = 0;

    char resultName[20] = "";
    char resultBlock[20] = "";
    char resultAutor[20] = "";

    newArt art;

    char artName[20] = "";
    char artBlock[20] = "";
    char artAutor[20] = "";

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //Aqui vai meu código

        ImGui::SetNextWindowSize(ImVec2(500, 500));
        if (ImGui::Begin("Search for Art Pieces in Museum", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
        {
            ImGui::Text("\n");
            ImGui::InputText("##inputname", inputName, IM_ARRAYSIZE(inputName));

            if (ImGui::Button("Search"))
            {
                cout << inputName << endl;
                if (inputName != NULL)
                {
                    newArt artItem;
                    toLower(inputName);
                    int idToSearch = convertToId(inputName);
                    int rrnFind = arvore->searchId(arvore->root, idToSearch);
                    if (rrnFind != -1) {
                        cout << rrnFind << endl;
                        artItem = returnItem(rrnFind);
                        cout << "The name is: " << artItem.name << endl;
                        searched = true;

                        stringToString(resultName, artItem.name);
                        stringToString(resultBlock, artItem.block);
                        stringToString(resultAutor, artItem.autor);

                        ImGui::Image((void*)my_texture, ImVec2(my_image_width, my_image_height));
                    }
                    clearChar(inputName);


                }
            }
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "\n\nLogin to add new pieces\n");
            ImGui::Text("Username:");
            ImGui::InputText("##usernameinput", inputUsername, CHAR_MAX);
            ImGui::Text("Password:");
            ImGui::InputText("##passwordinput", inputPassword, CHAR_MAX, ImGuiInputTextFlags_Password);

            if (ImGui::Button("Enter"))
            {
                if (!strcmp(inputUsername, username) && !strcmp(inputPassword, password))
                {
                    clearChar(inputUsername);
                    clearChar(inputPassword);
                    cout << "senha certa" << endl;
                    logged = true;

                }
            }

        } ImGui::End();

        if (logged)
        {
            ImGui::SetNextWindowSize(ImVec2(300, 400));
            if (ImGui::Begin("Add new Art Pieces", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu(""))
                    {
                        if (ImGui::MenuItem("Close")) { logged = false; }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }
                ImGui::Text("Add your new art here:\n");
                ImGui::Text("Name:");
                ImGui::InputText("##newartname", artName, CHAR_MAX);
                ImGui::Text("Block:");
                ImGui::InputText("##newartblock", artBlock, CHAR_MAX);
                ImGui::Text("Autor:");
                ImGui::InputText("##newartautor", artAutor, CHAR_MAX);
                if (ImGui::Button("Add"))
                {
                    toLower(artName);
                    arvore->root = arvore->insert(arvore->root, convertToId(artName), rrn);
                    newArt art;

                    toLower(artBlock);
                    toLower(artAutor);
                    stringToString(art.name, artName);
                    stringToString(art.block, artBlock);
                    stringToString(art.autor, artAutor);
                    art.rrn = rrn;
                    writeToFile(art);

                    rrn++;


                    //Clear inputs
                    clearChar(artName);
                    clearChar(artBlock);
                    clearChar(artAutor);



                }
                if (ImGui::Button("Print"))
                {
                    arvore->printInorder(arvore->root);
                    readFromFIle();
                }


            }ImGui::End();

        }

        if (searched)
        {
            ImGui::SetNextWindowSize(ImVec2(150, 150));
            if (ImGui::Begin("Masterpiece", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu(""))
                    {
                        if (ImGui::MenuItem("Close")) { searched = false; }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }
                ImGui::Text(resultName);
                ImGui::Text(resultBlock);
                ImGui::Text(resultAutor);




            }ImGui::End();
        }

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

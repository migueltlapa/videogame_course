#include "DXApp.h"
#include "Sprite.h"
#include <string.h>
#include <sstream>
class TestApp : public DXApp
{
public: 
	TestApp(HINSTANCE hInstance);
	~TestApp();

	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;

private:
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	ID3D11ShaderResourceView *m_pTexture;
	Sprite* sprite;
	int change_color = 0;       // Variable that permit to control the background color
	int change_full_screen = 0; // Variable that use to control the full screen
	int show_hide_sprite = 0;   // Variable that use to show and hide the sprite 
	float pos_x = 30;		    // Variable that use to define the min value of pos_x
	float pos_y = 30;			// Variable that use to define the min value of pos_Y
};


TestApp::TestApp(HINSTANCE hInstance) : DXApp(hInstance)
{

}

TestApp::~TestApp()
{

}

bool TestApp::Init()
{
	
	DXApp::Init();// Crea la ventana 
	//create spritebatch object
	spriteBatch.reset(new DirectX::SpriteBatch(m_pImmediateContext));
	//import texture for rendering
	sprite = new Sprite(DirectX::SimpleMath::Vector2(30, 30));
	sprite->Load(m_pDevice, L"image2.dds");
	


	return true;
}

void TestApp::Update(float dt)
{
	
	
	if (GetAsyncKeyState('A'))
	{

		m_Viewport[0].TopLeftX = 50;					// Defino la Posicion Top LEFTX del Viewport [0]
		m_Viewport[0].TopLeftY = 50;					// Defino la Posicion Top LEFTY del Viewport [0]
		//m_Viewport[0].Width = static_cast<float> (300);  // Defino el tamano del Ancho del Viewport [0]
		//m_Viewport[0].Height = static_cast<float> (300); // Defino el tamano del Alto del Viewport [0]
		
		
		m_Viewport[1].TopLeftX = 350;  
		m_Viewport[1].TopLeftY = 350;
		//m_Viewport[1].Width = static_cast<float> (300);
		//m_Viewport[1].Height = static_cast<float> (300);


	}
	
	if (GetAsyncKeyState('B'))
	{
		
		
		//change_color = ~change_color;
		change_color++;
		
		if (change_color == 4)
		{
			change_color = 0;
		}
	}
	if (GetAsyncKeyState('C'))
	{

		if (change_full_screen == 0)
		{
			m_pSwapChain->SetFullscreenState(TRUE, NULL);
			change_full_screen = ~change_full_screen;
		}
		
		else
		{
			m_pSwapChain->SetFullscreenState(FALSE, NULL);
			change_full_screen = ~change_full_screen;
		}
		

		
	}
	if (GetAsyncKeyState('D'))
	{
		std::ostringstream outs;
		outs << "GetAsyncKeyState OurApp";
		SetWindowText(m_hAppWnd, outs.str().c_str());

	}
	
	if (GetAsyncKeyState('T'))
	{
		std::ostringstream outs;
		outs << "OurApp";
		sprite->setPosition(DirectX::SimpleMath::Vector2(150, 150));
	}

	// Change the Sprite
	if (GetAsyncKeyState('W'))
	{
		
		show_hide_sprite = ~show_hide_sprite;


	}

	if (GetAsyncKeyState('R'))
	{
		pos_x = pos_x + 1;
		
		if (pos_x == 200)
		{
			pos_x = pos_x -1;

		}

	}

	if (GetAsyncKeyState('L'))
	{
		
		if (pos_x == 30)
		{
			pos_x = pos_x + 1;

		}
		pos_x = pos_x - 1;


	}

	if (GetAsyncKeyState('S'))
	{
		pos_y = pos_y + 1;
		if (pos_y == 200)
		{
			pos_y = pos_y - 1;

		}


	}

	if (GetAsyncKeyState('U'))
	{
		if (pos_y == 30)
		{
			pos_y = pos_y + 1;

		}
		pos_y = pos_y - 1;


	}





}

void TestApp::Render(float dt)
{
	
	if (change_color == 0)
	{
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Cyan);// No cambiar su posicion
		
	}
	
	if (change_color == 1)
	{
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Chocolate);// No cambiar su posicion
	}

	if (change_color == 2)
	{
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Black);// No cambiar su posicion
	}

	if (change_color == 3)
	{
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Green);// No cambiar su posicion
	
	}

	m_pImmediateContext->RSSetViewports(1, &m_Viewport[0]);
	
	if (show_hide_sprite == 0)
		{
		
		sprite->setPosition(DirectX::SimpleMath::Vector2(pos_x, pos_y));
		spriteBatch->Begin();
		sprite->Draw(spriteBatch.get());
		spriteBatch->End();
		}

	m_pImmediateContext->RSSetViewports(1, &m_Viewport[1]);
	
	if (show_hide_sprite == 0)
	{
		sprite->setPosition(DirectX::SimpleMath::Vector2(pos_x, pos_y));
		spriteBatch->Begin();
		sprite->Draw(spriteBatch.get());
		spriteBatch->End();
	}
	
	m_pSwapChain->Present(0, 0);// No cambiar su posicion
}


int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	TestApp tApp(hInstance);
	if (!tApp.Init()) return 1;

	//MessageBox(NULL, "Hello, World", "Test", NULL);
	return tApp.Run(); 
}


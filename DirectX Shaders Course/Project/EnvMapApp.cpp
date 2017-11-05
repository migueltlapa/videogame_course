#include "Framework\DXApp.h"
#include "Framework\Camera.h"
#include "Framework\MathHelper.h"
#include "Framework\LightHelper.h"
#include "Framework\UtilStructures.h"
#include "Framework\Waves.h"
#include "D3DCompiler.h"
#include <Effects.h> 

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <fstream>
#include <vector>
#include "Framework\GeometryGenerator.h"
#include "RenderStates.h"
#include "ShadowMap.h"
#include "Framework\objloader.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment (lib, "D3DCompiler.lib")


#define CAM_SIZE 6

class EnvMapApp : public DXApp{
public:
	EnvMapApp(HINSTANCE hInstance);
	~EnvMapApp();

	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;
	void Render(const Camera &camera, bool drawCenterSphere);
	void OnResize() override;

private:

	//input layout
	ID3D11InputLayout	*mpLayout;           // the pointer to the input layout

	//shaders
	ID3D11VertexShader	*mpVS;               // the pointer to the vertex shader
	ID3D11PixelShader	*mpPS;               // the pointer to the pixel shader
	ID3D11PixelShader	*mpSkyPS;               // the pointer to the SKY pixel shader

	//Buffers
	ID3D11Buffer		*mpModelVB;          // the pointer to the vertex buffer
	ID3D11Buffer		*mpSkullIB;

	ID3D11Buffer		*mShapesVB;
	ID3D11Buffer		*mShapesIB;

	//ROOM
	ID3D11Buffer		*mpRoomVB;	 // the pointer to the vertex buffer for the room
	XMFLOAT4X4			 mRoomWorld;
	ComplexMaterial		 mRoomMat;
	ComplexMaterial      mMirrorMat;

	ID3D11ShaderResourceView *mpSRV_Wall;
	ID3D11ShaderResourceView *mpSRV_mirror;

	//SNAKE
	ID3D11ShaderResourceView *mpSRV_Snake;
	ID3D11Buffer *mpSnakeVB;

	//Spheres
	ID3D11Buffer		*mpSphereVB;
	ID3D11Buffer		*mpSphereIB;
	XMFLOAT4X4 mSphereWorld;
	XMFLOAT4X4 mSkyWorld;

	ComplexMaterial mSphereMat;
	ComplexMaterial mSkyMat;

	UINT	mModelIndexCount;
	UINT	mSmallSphereIndexCount;

	//AIR BOX
	//Spheres
	ID3D11Buffer		*mpAirBoxVB;
	ID3D11Buffer		*mpAirBoxIB;
	float change_airbox;
	ID3D11PixelShader	*mpNewShader;
	bool key_ChangeShader;
	
	// Apuntador de Textura del Craneo
	ID3D11ShaderResourceView* 		mpSRV_Skull;

	// Textura Skull
	float textura_x;
	float textura_y;


	ObjReader *mObjReader;
	ComplexMaterial mSnakeMat;
	XMFLOAT4X4 mSnakeWorld;

	UINT mSnakeIndexCount;
	UINT mSnakeVertexCount;


	UINT	mAirBoxIndexCount;

	// Rasterizer Stage
	int count_rasterizer_state;


	//rasterizer state
	ID3D11RasterizerState* mpWireframeRS;

	//constant buffers
	enum ConstantBuffer
	{
		CB_Object,
		NumConstantBuffers   //must be the last
	};

	ID3D11Buffer* mpConstantBuffers[NumConstantBuffers];

	enum PSConstantBuffer
	{
		CB_EyeInfo,
		CB_LightInfo,
		CB_MaterialInfo,
		CB_FogInfo,
		PSNumConstantBuffers   //must be the last
	};


	ID3D11Buffer* mpConstP[PSNumConstantBuffers];



	//Matrices
	XMFLOAT4X4 mSkullWorld;
	ComplexMaterial mSkullMat;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProjection;

	XMFLOAT4X4 mShapeSphereWorlds[10];
	XMFLOAT4X4 mShapeCylWorlds[10];
	XMFLOAT4X4 mShapeBoxWorld;
	XMFLOAT4X4 mShapeGridWorld;
	XMFLOAT4X4 mAirBoxWorld;

	ComplexMaterial mShapeSphereMat;
	ComplexMaterial mShapeCylMat;
	ComplexMaterial mShapeBoxMat;
	ComplexMaterial mShapeGridMat;
	ComplexMaterial mAirBoxMat;


	//
	float mAngle;
	float SkullMAngle;
	float skullXpos;
	float skullYpos;
	float skullZpos;

	float sphereXpos;
	float sphereYpos;
	float sphereZpos;


	boolean spotLightON = true;

	//camera
	Camera mCam;

	POINT mLastMousePos;

	Light gLight;

	ID3D11SamplerState*			mpSamplerState;
	ID3D11ShaderResourceView*	mpSRV_FloorTex; //SAVE THE TEXTURES
	ID3D11ShaderResourceView*	mpSRV_FloorTexNMap;
	ID3D11ShaderResourceView*	mpSRV_SphereCubeMap;

	ID3D11ShaderResourceView*	mpSRV_Teapot;
	ID3D11ShaderResourceView*	mpSRV_AirBoxTexture2;
	ID3D11ShaderResourceView*	mpSRV_AirBoxOneMap;

	Fog mFog;
	// change SpotLight

	int posicion_spotlight;
	int angle_spotlight_0;


	// Enable Angulo Calaca
	int key_enable_angle_calaca;
	int enable_angle_calaca ;

	// Enable / Disable Texture
	int enable_texture = 1;
	int key_enable_texture;

	// Enable / Disable Lights

	int enable_lights = 1;
	int key_enable_lights;

	//Light Positions
	float m_light_x = 0.0f;
	float m_light_y = 0.0f;
	float m_light_z = -10.0f;

	//CAR
	ComplexMaterial mCarmat;

	ID3D11Buffer *mpCarVB;
	ID3D11Buffer *mpCarIB;
	XMFLOAT4X4 mCarWorld;
	float m_Car_x = -15;

	float m_Car_y = 0.0f;
	UINT mCarIndexCount;

	//TEAPOT
	ComplexMaterial mTeapotmat;

	ID3D11Buffer *mpTeapotVB;
	ID3D11Buffer *mpTeapotIB;
	XMFLOAT4X4 mTeapotWorld;

	float m_Teapot_y = 0.0f;
	UINT mTeapotIndexCount;

	// Water 2
	ID3D11ShaderResourceView* 		mpSRV_Water2;
	// Waves
	ComplexMaterial mWavesMat;
	ID3D11Buffer* mWavesVB;
	ID3D11Buffer* mWavesIB;
	Waves mWaves;
	XMFLOAT4X4 mWavesWorld;
	ID3D11ShaderResourceView* 		mpSRV_Water;

	// Apuntador de Textura de la Caja
	ID3D11ShaderResourceView* 		mpSRV_Boxer;
	ID3D11ShaderResourceView* mpSRV_Boxer_Array[120];

	// Variable Incrementa Frame del BOX
	int increment_frame;
	int pass_first_frame;





	// DYNAMIC CUBE MAP
	Camera mCubeMapCamera[CAM_SIZE];							//six cameras 
	ID3D11DepthStencilView*		mpDynamicCubeMapDSV;			// depth/Stencil used for rendering the dynamic cube map 
	ID3D11RenderTargetView*		mpDynamicCubeMapRTV[CAM_SIZE];	// six render target views 
	ID3D11ShaderResourceView*	mpDynamicCubeMapSRV;			// the new cube map 
	D3D11_VIEWPORT		mCubeMapViewport;						// a new viewport 
	static const int	CubeMapSize = 256;						// small texture images
	UINT mBoxVertexOffset;
	UINT mGridVertexOffset;
	UINT mSphereVertexOffset;
	UINT mCylinderVertexOffset;
	UINT mBoxIndexCount;
	UINT mGridIndexCount;
	UINT mCylinderIndexCount;
	UINT mBoxIndexOffset;
	UINT mGridIndexOffset;
	UINT mSphereIndexOffset;
	UINT mCylinderIndexOffset;
	UINT mSphereIndexCount;
	ID3D11ShaderResourceView* mpSRV_GrassTex;
	ID3D11ShaderResourceView* mpSRV_BricksTex;
private:
	void InitPrecompiledShaders();
	void InitBuffers();
	void InitConstantBuffers();
	void InitAirBoxBuffers();
	void InitTextures();
	void InitRoomBuffer();
	void InitCarBuffers();
	void InitTeapotBuffers();
	void InitSpheres();
	void BuildDynamicCubeMapViews();
	void BuildCubeFaceCamera(float x, float y, float z);
	void InitWaveBuffers();
	
	void InitRasterizerState();

	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseDown(WPARAM btnState, int x, int y);
	void BuildShapeGeometryBuffers();
};

EnvMapApp::EnvMapApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_appTitle = "MirrorApp";

	//mouse
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	//camera
	mCam.SetPosition(0.0f, 4.0f, -10.0f);

	//skull CRYSTAL material
	mSkullMat.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSkullMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 0.35f);
	mSkullMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mSkullMat.SpecularPower = 100.0f;
	mSkullMat.Reflect = XMFLOAT4(0.2, 0.34, 0.2, 0.4);
	mSkullMat.EnableReflection = 1;
	mSkullMat.UseTexture = 1;

	//skull regular reflective material
	//mSkullMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//mSkullMat.Diffuse = XMFLOAT4(0.3f, 0.4f, 0.2f, 1.0f);
	//mSkullMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//mSkullMat.SpecularPower = 100.0f;
	//mSkullMat.Reflect = XMFLOAT4(0.65, 0.65, 0.65, 0.65);
	//mSkullMat.EnableReflection = 1;
	//mSkullMat.UseTexture = 0;

	//room material
	mRoomMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mRoomMat.Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 0.5f);
	mRoomMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mRoomMat.SpecularPower = 16.0f;
	mRoomMat.Reflect = XMFLOAT4(0.4, 0.4, 0.4, 0.4);
	mRoomMat.EnableReflection = 1;
	mRoomMat.UseTexture = 1;

	//mirror material
	mMirrorMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mMirrorMat.Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 0.5f);
	mMirrorMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mMirrorMat.SpecularPower = 16.0f;
	mMirrorMat.Reflect = XMFLOAT4(0.4, 0.4, 0.4, 0.4);
	mMirrorMat.EnableReflection = 0;
	mMirrorMat.UseTexture = 1;

	//sphere material
	mSphereMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mSphereMat.Diffuse = XMFLOAT4(0.3f, 0.4f, 0.2f, 1.0f);
	mSphereMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSphereMat.SpecularPower = 16.0f;
	mSphereMat.Reflect = XMFLOAT4(0.4, 0.4, 0.4, 0.4);
	mSphereMat.EnableReflection = 1;
	mSphereMat.Refract = XMFLOAT4(0.4, 0.4, 0.4, 0.9);
	mSphereMat.EnableRefraction = 0;
	mSphereMat.UseTexture = 0;

	//AirBox material
	mAirBoxMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mAirBoxMat.Diffuse = XMFLOAT4(0.3f, 0.4f, 0.2f, 1.0f);
	mAirBoxMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mAirBoxMat.SpecularPower = 16.0f;
	mAirBoxMat.Reflect = XMFLOAT4(0.4, 0.4, 0.4, 0.4);
	mAirBoxMat.EnableReflection = 1;
	mAirBoxMat.Refract = XMFLOAT4(0.4, 0.4, 0.4, 0.9);
	mAirBoxMat.EnableRefraction = 0;
	mAirBoxMat.UseTexture = 2;
	mAirBoxMat.FusionAlpha = 0.3;
	change_airbox = 0.001;

	//CarMaterial
	mCarmat.Ambient = XMFLOAT4(0.5, 0.7, 0.5, 1.0f);
	mCarmat.Diffuse = XMFLOAT4(0.5, 0.5, 0.5, 1.0f);
	mCarmat.Specular = XMFLOAT4(0.2, 0.2, 0.2, 1.0f);
	mCarmat.SpecularPower = 16.0f;
	mCarmat.Reflect = XMFLOAT4(0.2, 0.34, 0.2, 0.4);
	mCarmat.EnableReflection = 1;
	mCarmat.UseTexture = 0.0;


	//TeapotMaterial
	mTeapotmat.Ambient = XMFLOAT4(0.5, 0.7, 0.5, 1.0f);
	mTeapotmat.Diffuse = XMFLOAT4(0.5, 0.5, 0.5, 1.0f);
	mTeapotmat.Specular = XMFLOAT4(0.2, 0.2, 0.2, 1.0f);
	mTeapotmat.SpecularPower = 16.0f;
	mTeapotmat.Reflect = XMFLOAT4(0.2, 0.34, 0.2, 0.4);
	mTeapotmat.EnableReflection = 1;
	mTeapotmat.UseTexture = 1;
	
	//WAVES

	XMMATRIX W5 = XMMatrixTranslation(0, -3, 0);
	XMStoreFloat4x4(&mWavesWorld, W5);
	mWavesMat.Ambient = XMFLOAT4(1.0, 1.0, 1.0, 1.0f);
	mWavesMat.Diffuse = XMFLOAT4(0.5, 0.7, 0.5, 1.0f);
	mWavesMat.Specular = XMFLOAT4(0.2, 0.2, 0.2, 1.0f);
	mWavesMat.SpecularPower = 16.0f;
	mWavesMat.UseTexture = 1.0;

	//-----------SHAPES MATERIALS----------
	mShapeSphereMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mShapeSphereMat.Diffuse = XMFLOAT4(0.3f, 0.4f, 0.2f, 1.0f);
	mShapeSphereMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mShapeSphereMat.SpecularPower = 16.0f;
	mShapeSphereMat.Reflect = XMFLOAT4(0.4, 0.4, 0.4, 0.4);
	mShapeSphereMat.EnableReflection = 1;
	mShapeSphereMat.Refract = XMFLOAT4(0.4, 0.4, 0.4, 0.9);
	mShapeSphereMat.EnableRefraction = 0;
	mShapeSphereMat.UseTexture = 1;

	mShapeCylMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mShapeCylMat.Diffuse = XMFLOAT4(0.3f, 0.4f, 0.2f, 1.0f);
	mShapeCylMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mShapeCylMat.SpecularPower = 16.0f;
	mShapeCylMat.Reflect = XMFLOAT4(0.4, 0.4, 0.4, 0.4);
	mShapeCylMat.EnableReflection = 1;
	mShapeCylMat.Refract = XMFLOAT4(0.4, 0.4, 0.4, 0.9);
	mShapeCylMat.EnableRefraction = 0;
	mShapeCylMat.UseTexture = 1;

	mShapeBoxMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mShapeBoxMat.Diffuse = XMFLOAT4(0.3f, 0.4f, 0.2f, 1.0f);
	mShapeBoxMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mShapeBoxMat.SpecularPower = 16.0f;
	mShapeBoxMat.Reflect = XMFLOAT4(0.4, 0.4, 0.4, 0.4);
	mShapeBoxMat.EnableReflection = 1;
	mShapeBoxMat.Refract = XMFLOAT4(0.4, 0.4, 0.4, 0.9);
	mShapeBoxMat.EnableRefraction = 0;
	mShapeBoxMat.UseTexture = 0;

	mShapeGridMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mShapeGridMat.Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 0.5f);
	mShapeGridMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mShapeGridMat.SpecularPower = 16.0f;
	mShapeGridMat.Reflect = XMFLOAT4(0.4, 0.4, 0.4, 0.4);
	mShapeGridMat.EnableReflection = 1;
	mShapeGridMat.UseTexture = 1;
	// --------- END SHAPES MATERIALS ---------


	XMStoreFloat4x4(&mShapeBoxWorld,
		XMMatrixScaling(3, 1, 3) *
		XMMatrixTranslation(0, 0.5, 0));

	for (int i = 0; i < 5; ++i)
	{
		XMStoreFloat4x4(&mShapeCylWorlds[i * 2 + 0], XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i*5.0f));
		XMStoreFloat4x4(&mShapeCylWorlds[i * 2 + 1], XMMatrixTranslation(+5.0f, 1.5f, -10.0f + i*5.0f));

		XMStoreFloat4x4(&mShapeSphereWorlds[i * 2 + 0], XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i*5.0f));
		XMStoreFloat4x4(&mShapeSphereWorlds[i * 2 + 1], XMMatrixTranslation(+5.0f, 3.5f, -10.0f + i*5.0f));
	}

	//SKY material
	mSkyMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mSkyMat.Diffuse = XMFLOAT4(0.3f, 0.4f, 0.2f, 1.0f);
	mSkyMat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSkyMat.SpecularPower = 16.0f;
	mSkyMat.UseTexture = 1;

	mAngle = 0.0f;

	XMMATRIX I = XMMatrixIdentity();
	
	XMStoreFloat4x4(&mSkullWorld, I);
	XMStoreFloat4x4(&mSphereWorld, I);
	XMStoreFloat4x4(&mSkyWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProjection, I);
	XMMATRIX W2 = XMMatrixTranslation(0, 0, 5);
	XMStoreFloat4x4(&mRoomWorld, W2);
	XMStoreFloat4x4(&mShapeGridWorld, I);
	XMMATRIX W = XMMatrixTranslation(0, 6, 0);
	XMStoreFloat4x4(&mAirBoxWorld, W);

	XMStoreFloat4x4(&mCarWorld, I);
	XMStoreFloat4x4(&mTeapotWorld, I);


	mFog.Color = GColors::Silver;
	mFog.Start = 10.0f;
	mFog.Range = 70.0f;
	mFog.Enable = 1;

	gLight.Color = XMFLOAT4(1, 1, 1, 1);
	gLight.Kc = 1.0f;
	gLight.Kl = 0.01f;
	gLight.Kq = 0.02f;
	gLight.SpotAngle = XMConvertToRadians(15);
	gLight.Enabled = 1;

	skullXpos = 0;
	skullYpos = 1;
	skullZpos = -1;

	sphereXpos = 2;
	sphereYpos = 2;
	sphereZpos = 0;
	//BuildCubeFaceCamera(sphereXpos, sphereYpos, sphereZpos);
	BuildCubeFaceCamera(skullXpos, skullYpos, skullZpos);


	for (int i = 0; i < CAM_SIZE; i++)
		mpDynamicCubeMapRTV[i] = 0;

	XMStoreFloat4x4(&mSnakeWorld, XMMatrixScaling(5, 5, 5)*XMMatrixTranslation(0, 4, 0));

}



EnvMapApp::~EnvMapApp()
{
	Memory::SafeRelease(mpLayout);

	Memory::SafeRelease(mpVS);
	Memory::SafeRelease(mpPS);

	Memory::SafeRelease(mpModelVB);
	Memory::SafeRelease(mpSkullIB);

	Memory::SafeRelease(mpConstantBuffers[CB_Object]);

	Memory::SafeRelease(mpConstP[CB_EyeInfo]);
	Memory::SafeRelease(mpConstP[CB_LightInfo]);
	Memory::SafeRelease(mpConstP[CB_MaterialInfo]);

	Memory::SafeRelease(mpRoomVB);
	Memory::SafeRelease(mpSphereIB);
	Memory::SafeRelease(mpSphereVB);

	Memory::SafeRelease(mpSRV_FloorTex);
	Memory::SafeRelease(mpSRV_SphereCubeMap);

	Memory::SafeRelease(mpDynamicCubeMapDSV);
	for (int i = 0; i < CAM_SIZE; i++)
		Memory::SafeRelease(mpDynamicCubeMapRTV[i]);

	Memory::SafeRelease(mpDynamicCubeMapSRV);

}

void EnvMapApp::OnResize(){
	DXApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	XMMATRIX P = mCam.Proj();
	//XMMATRIX P = mCam.get XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_ClientWidth / m_ClientHeight, 0.1f, 100.0f);
	XMStoreFloat4x4(&mProjection, P);

}

bool EnvMapApp::Init()
{
	if (!DXApp::Init())
		return false;

	InitRoomBuffer();
	InitBuffers();
	InitWaveBuffers();
	BuildShapeGeometryBuffers();
	InitPrecompiledShaders();
	InitConstantBuffers();
	InitRasterizerState();
	InitTextures();
	InitRoomBuffer();
	InitAirBoxBuffers();
	InitCarBuffers();
	InitTeapotBuffers();
	InitSpheres();
	BuildDynamicCubeMapViews();
	RenderStates::InitAll(m_pDevice);
	OnResize();

	XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);

	XMVECTOR pos = XMLoadFloat3(&mCam.GetPosition());
	mCam.LookAt(pos, focusPoint, upDirection);

	return true;
}

void EnvMapApp::Update(float dt)
{
	//
	// Control the camera.
	//
	if (GetAsyncKeyState('W') & 0x8000)
		mCam.Walk(10.0f*dt);

	if (GetAsyncKeyState('S') & 0x8000)
		mCam.Walk(-10.0f*dt);

	if (GetAsyncKeyState('A') & 0x8000)
		mCam.Strafe(-10.0f*dt);

	if (GetAsyncKeyState('D') & 0x8000)
		mCam.Strafe(10.0f*dt);

	if (GetAsyncKeyState(VK_UP) & 0x8000){
		skullZpos += 0.02;
		//sphereZpos += 0.01;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000){
		skullZpos -= 0.02;
		//sphereZpos -= 0.01;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000){
		skullXpos -= 0.02;
		//sphereXpos -= 0.01;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000){
		skullXpos += 0.02;
		//sphereXpos += 0.01;
	}

	if (GetAsyncKeyState('C') & 0x01){
		gLight.Enabled = spotLightON == true ? 1 : 0;
		spotLightON = !spotLightON;
	}

	if (GetAsyncKeyState('P') & 0x01){
		
		key_ChangeShader = !key_ChangeShader;
	}

	if (GetAsyncKeyState('J') & 0x01)

	{

		if (key_enable_texture == 0)

		{

			mSkullMat.UseTexture = 0.0;
		
			
			mRoomMat.UseTexture = 0.0;
			enable_texture = 0;
			key_enable_texture = ~key_enable_texture;



		}

		else
		{

			mSkullMat.UseTexture = 1.0;
			mRoomMat.UseTexture = 1.0;
			
			enable_texture = 1;
			key_enable_texture = ~key_enable_texture;


		}


	}

	if (GetAsyncKeyState('F') & 0x01)

	{

		if (key_enable_angle_calaca == 0)

		{

			enable_angle_calaca = 0;
			key_enable_angle_calaca = ~key_enable_angle_calaca;



		}

		else
		{

		
			enable_angle_calaca = 1;
			key_enable_angle_calaca = ~key_enable_angle_calaca;


		}


	}

	using namespace DirectX;
	using namespace DirectX::PackedVector;


	//XMVECTOR eyePosition = XMVectorSet(0, 0, -10, 1);
	/*if (GetAsyncKeyState('A'))
	eyePosition = XMVectorSet(0, 0, -10 + ((int)(mAngle) % 100) + 0.7f, 1);*/

	mCam.UpdateViewMatrix();
	XMMATRIX V = mCam.View();
	//XMMATRIX V = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
	XMStoreFloat4x4(&mView, V);
	//m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Frame], 0, nullptr, &mView, 0, 0);


	XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);

	//mAngle += 90.0f*dt;
	XMMATRIX W = XMMatrixScaling(0.5, 0.5, 0.5);
	W *= XMMatrixTranslation(skullXpos, skullYpos, skullZpos);
	//W *= XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(mAngle));
	XMStoreFloat4x4(&mSkullWorld, W);


	//SphereWorld
	mAngle += 90.0f*dt;
	XMMATRIX Wsphere = XMMatrixTranslation(sphereXpos, sphereYpos, sphereZpos);
	Wsphere *= XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(mAngle));
	Wsphere *= XMMatrixScaling(3, 3, 3);
	XMStoreFloat4x4(&mSphereWorld, Wsphere);

	XMMATRIX W22 = XMMatrixScaling(0.5f, 0.5f, 0.5f)*
		XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(mAngle*enable_angle_calaca)) *
		XMMatrixTranslation(0.00f, 1.0f, -5.0f);


	XMStoreFloat4x4(&mSkullWorld, W22);


	XMFLOAT3 Epos = mCam.GetPosition();
	XMFLOAT4 EyePos = XMFLOAT4(Epos.x, Epos.y, Epos.z, 1.0f);

	XMFLOAT3 Edir = mCam.GetLook();
	XMFLOAT4 EyeDir = XMFLOAT4(Edir.x, Edir.y, Edir.z, 1.0f);

	EYE eyeInfo;
	eyeInfo.EyePos = EyePos;
	eyeInfo.EyeDir = EyeDir;

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_EyeInfo], 0, nullptr, &eyeInfo, 0, 0);

	// Defining the Light structure

	gLight.Position = EyePos;
	gLight.Direction = EyeDir;

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_LightInfo], 0, nullptr, &gLight, 0, 0);


	if (GetAsyncKeyState('H') & 0x01)
	{
		if (count_rasterizer_state == 0)
		{

			D3D11_RASTERIZER_DESC wireframeDesc;
			ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
			wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_WIREFRAME; //
			wireframeDesc.CullMode = D3D11_CULL_NONE;
			wireframeDesc.FrontCounterClockwise = false;
			wireframeDesc.DepthClipEnable = true;
			HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mpWireframeRS));

			count_rasterizer_state = ~count_rasterizer_state;

		}

		else
		{

			D3D11_RASTERIZER_DESC wireframeDesc;
			ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
			wireframeDesc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME; //
			wireframeDesc.CullMode = D3D11_CULL_NONE;
			wireframeDesc.FrontCounterClockwise = false;
			wireframeDesc.DepthClipEnable = true;
			HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mpWireframeRS));

			count_rasterizer_state = ~count_rasterizer_state;


		}



	}







	// ENable SpotLight
	if (GetAsyncKeyState('Y') & 0x01)

	{

		if (posicion_spotlight == 0)

		{

			angle_spotlight_0 = 5;
			posicion_spotlight = ~posicion_spotlight;

		}

		else
		{

			angle_spotlight_0 = 0;
			posicion_spotlight = ~posicion_spotlight;

		}


	}

	// ENable Lights
	if (GetAsyncKeyState('K') & 0x01)

	{

		if (key_enable_lights == 0)

		{

			enable_lights = 0;
			key_enable_lights = ~key_enable_lights;
		}

		else
		{
			enable_lights = 1;
			key_enable_lights = ~key_enable_lights;

		}


	}




	// Traer el Valor de Vcount

	// Change the value of Texture
	static float t_base3 = 0.0f;

	if ((mTimer.TotalTime() - t_base3) >= 10.0f)
	{
		t_base3 += 20.0f;
		textura_x += 0.4;
		textura_y += 0.4;
		InitBuffers();
	}

	if (textura_x == 1.0)
	{
		textura_x = 0.1;
		textura_y = 0.1;


	}



	//Sky world
	XMMATRIX W2 = XMMatrixScaling(200, 200, 200);
	W2 *= XMMatrixTranslation(EyePos.x, EyePos.y, EyePos.z);
	XMStoreFloat4x4(&mSkyWorld, W2);

	// Move Car
	m_Car_x += 0.001;
	XMMATRIX d = XMMatrixTranslation(m_Car_x, 5, 6);
	XMStoreFloat4x4(&mCarWorld, d);

	// Move Teapot
	XMMATRIX d2 = XMMatrixTranslation(-20, 3, 2);
	XMStoreFloat4x4(&mTeapotWorld, d2);

	


	//Light
	Light gLight[6];
	m_light_x += 0.01;
	//gLight[0].Position = XMFLOAT4(m_light_x, 4.0f, -10.0f, 1.0f);
	gLight[0].Position = EyePos;

	gLight[0].Direction = EyeDir;
	gLight[0].Color = XMFLOAT4(1,1, 1, 1);    // Main Light Cono
	gLight[0].Kc = 1.0f;
	gLight[0].Kl = 0.01f;
	gLight[0].Kq = 0.02f;
	gLight[0].SpotAngle = XMConvertToRadians(angle_spotlight_0);
	gLight[0].Enabled = 1;

	gLight[1].Position = XMFLOAT4(m_light_x + 2, 4.0f, -10.0f, 1.0f);
	gLight[1].Direction = EyeDir;
	gLight[1].Color = XMFLOAT4(0, 1, 0, 1);   // Green
	gLight[1].Kc = 1.0f;
	gLight[1].Kl = 0.01f;
	gLight[1].Kq = 0.02f;
	gLight[1].SpotAngle = XMConvertToRadians(15 * enable_lights);
	gLight[1].Enabled = 1;
	if (m_light_x>= 5)
	{
		m_light_x = 0;
	}

	m_light_y += 0.001;
	gLight[2].Position = XMFLOAT4(0.0f, m_light_y, -10.0f, 1.0f);
	gLight[2].Direction = EyeDir;
	gLight[2].Color = XMFLOAT4(0, 0, 1, 1);   //Blue
	gLight[2].Kc = 1.0f;
	gLight[2].Kl = 0.01f;
	gLight[2].Kq = 0.02f;
	gLight[2].SpotAngle = XMConvertToRadians(15 * enable_lights);
	gLight[2].Enabled = 1;


	gLight[3].Position = XMFLOAT4(0.0f, m_light_y + 2, -10.0f, 1.0f);
	gLight[3].Direction = EyeDir;
	gLight[3].Color = XMFLOAT4(1, 0, 0, 1);
	gLight[3].Kc = 1.0f;
	gLight[3].Kl = 0.01f;
	gLight[3].Kq = 0.02f;
	gLight[3].SpotAngle = XMConvertToRadians(15 * enable_lights);
	gLight[3].Enabled = 1;


	m_light_z -= 0.001;
	gLight[4].Position = XMFLOAT4(0.0f, 0.0f, m_light_z, 1.0f);
	gLight[4].Direction = EyeDir;
	gLight[4].Color = XMFLOAT4(1, 0, 0, 1);
	gLight[4].Kc = 1.0f;
	gLight[4].Kl = 0.01f;
	gLight[4].Kq = 0.02f;
	gLight[4].SpotAngle = XMConvertToRadians(15 * enable_lights);
	gLight[4].Enabled = 1;


	gLight[5].Position = XMFLOAT4(0.0f, 0.0f, m_light_z - 2, 1.0f);
	gLight[5].Direction = EyeDir;
	gLight[5].Color = XMFLOAT4(1, 0, 0, 1);
	gLight[5].Kc = 1.0f;
	gLight[5].Kl = 0.01f;
	gLight[5].Kq = 0.02f;
	gLight[5].SpotAngle = XMConvertToRadians(15 * enable_lights);
	gLight[5].Enabled = 1;
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_LightInfo], 0, nullptr, &gLight[0], 0, 0);

	// Waves
	static float t_base = 0.0f;
	if ((mTimer.TotalTime() - t_base) >= 0.25f)
	{
		t_base += 0.25f;
		DWORD i = 5 + rand() % (mWaves.RowCount() - 10);
		DWORD j = 5 + rand() % (mWaves.ColumnCount() - 10);
		float r = MathHelper::RandF(1.0f, 2.0f);
		mWaves.Disturb(i, j, r);
	}
	mWaves.Update(dt);
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pImmediateContext->Map(mWavesVB, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedData));
	VertexPNT* v = reinterpret_cast<VertexPNT*>(mappedData.pData);
	for (UINT i = 0; i < mWaves.VertexCount(); ++i)
	{
		v[i].Pos = mWaves[i];
		v[i].Normal = mWaves.Normal(i);
		// Derive tex-coords in [0,1] from position.
		v[i].Tex.x = (0.5f + mWaves[i].x / mWaves.Width());
		v[i].Tex.y = (0.5f - mWaves[i].z / mWaves.Depth());
	}
	m_pImmediateContext->Unmap(mWavesVB, 0);






}

void EnvMapApp::Render(float dt)
{

	//------ Create dynamic renders for cube map -------
	ID3D11RenderTargetView* renderTargets[1];
	m_pImmediateContext->RSSetViewports(1, &mCubeMapViewport);
	for (int i = 0; i < CAM_SIZE; i++)
	{
		//mCubeMapCamera[i].SetPosition(sphereXpos, sphereYpos, sphereZpos);
		mCubeMapCamera[i].SetPosition(skullXpos, skullYpos, skullZpos);
		mCubeMapCamera[i].UpdateViewMatrix();
		m_pImmediateContext->ClearRenderTargetView(mpDynamicCubeMapRTV[i], DirectX::Colors::CornflowerBlue);
		m_pImmediateContext->ClearDepthStencilView(mpDynamicCubeMapDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		//***********************
		renderTargets[0] = mpDynamicCubeMapRTV[i];
		m_pImmediateContext->OMSetRenderTargets(1, renderTargets, mpDynamicCubeMapDSV);
		Render(mCubeMapCamera[i], false);
	}
	//--------- end dynamic cube map --------------------


	//--------- restore old viewport and render target ------------
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);
	renderTargets[0] = m_pRenderTargetView;
	m_pImmediateContext->OMSetRenderTargets(1, renderTargets, m_pDepthStencilView);
	m_pImmediateContext->GenerateMips(mpDynamicCubeMapSRV);
	//--------- end restore ---------------------------------------

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::CornflowerBlue);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	Render(mCam, true);

	//***********************
	//float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//m_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);
	//m_pImmediateContext->PSSetSamplers(0, 1, &mpSamplerState);
	//m_pImmediateContext->IASetInputLayout(mpLayout);
	//// select which primtive type we are using
	//m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//// select which vertex buffer to display
	//UINT stride = sizeof(VertexPNT);
	//UINT offset = 0;
	////m_pImmediateContext->RSSetState(mpWireframeRS);
	////shaders
	//m_pImmediateContext->VSSetShader(mpVS, nullptr, 0);
	//m_pImmediateContext->VSSetConstantBuffers(0, NumConstantBuffers, mpConstantBuffers);
	//m_pImmediateContext->PSSetShader(mpPS, nullptr, 0);
	//m_pImmediateContext->PSSetConstantBuffers(0, PSNumConstantBuffers, mpConstP);
	//ObjectMatrices objMatx;
	////--------------------------BLEND STATE------------------
	//m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	////------------------------- SPHERES ---------------------
	////------------------------SKYSPHERE-----------------------
	//m_pImmediateContext->IASetVertexBuffers(0, 1, &mpSphereVB, &stride, &offset);
	//m_pImmediateContext->IASetIndexBuffer(mpSphereIB, DXGI_FORMAT_R32_UINT, 0);
	//// SET THE SKY SHADER
	//m_pImmediateContext->PSSetShader(mpSkyPS, 0, 0);
	////constant buffers of the vertex shader
	//objMatx.worldMatrix = XMLoadFloat4x4(&mSkyWorld);
	//objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	//objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	//objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//m_pImmediateContext->RSSetState(RenderStates::NoCullRS);
	//m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	//m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkyMat, 0, 0);
	//m_pImmediateContext->PSSetShaderResources(0, 1, &mpSRV_SphereCubeMap);
	//m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);
	////------------------------NORMAL SPHERE-----------------------
	//m_pImmediateContext->IASetVertexBuffers(0, 1, &mpSphereVB, &stride, &offset);
	//m_pImmediateContext->IASetIndexBuffer(mpSphereIB, DXGI_FORMAT_R32_UINT, 0);
	////RETURN TO NORMAL SHADER
	//m_pImmediateContext->PSSetShader(mpPS, nullptr, 0);
	////constant buffers of the vertex shader
	//objMatx.worldMatrix = XMLoadFloat4x4(&mSphereWorld);
	//objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	//objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	//objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//ID3D11ShaderResourceView *skyTexturesSRV[] = { mpSRV_FloorTex, mpSRV_SphereCubeMap };
	//m_pImmediateContext->PSSetShaderResources(0, 2, skyTexturesSRV);
	//m_pImmediateContext->RSSetState(0);
	//m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	//m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSphereMat, 0, 0);
	//m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);
	////-----------------------ROOM-------------------------
	//m_pImmediateContext->IASetVertexBuffers(0, 1, &mpRoomVB, &stride, &offset);
	////constant buffers of the vertex shader
	//objMatx.worldMatrix = XMLoadFloat4x4(&mRoomWorld);
	//objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	//objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	//objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	//m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mRoomMat, 0, 0);
	//ID3D11ShaderResourceView *floorTexturesSRV[] = { mpSRV_FloorTex, mpSRV_SphereCubeMap };
	//m_pImmediateContext->PSSetShaderResources(0, 2, floorTexturesSRV);
	//m_pImmediateContext->Draw(6, 0);
	//m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	////***********************
	////-----------------------SKULL-------------------------
	//m_pImmediateContext->IASetVertexBuffers(0, 1, &mpModelVB, &stride, &offset);
	//m_pImmediateContext->IASetIndexBuffer(mpSkullIB, DXGI_FORMAT_R32_UINT, 0);
	////constant buffers of the vertex shader
	//objMatx.worldMatrix = XMLoadFloat4x4(&mSkullWorld);
	//objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	//objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	//objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//m_pImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
	//ID3D11ShaderResourceView *skullTexturesSRV[] = { mpSRV_FloorTex, mpSRV_SphereCubeMap };
	//m_pImmediateContext->PSSetShaderResources(0, 2, skullTexturesSRV);
	//m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	//m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkullMat, 0, 0);
	//m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);
	//m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	////-------------------------END SKULL-----------------------

	HR(m_pSwapChain->Present(0, 0));
}

void EnvMapApp::Render(const Camera &camera, bool drawCenterSphere)
{
	XMMATRIX V = camera.View();
	XMStoreFloat4x4(&mView, V);
	XMStoreFloat4x4(&mProjection, camera.Proj());

	//***********************
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);
	m_pImmediateContext->PSSetSamplers(0, 1, &mpSamplerState);

	m_pImmediateContext->IASetInputLayout(mpLayout);
	// select which primtive type we are using
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// select which vertex buffer to display
	UINT stride = sizeof(VertexPNT);
	UINT offset = 0;

	//m_pImmediateContext->RSSetState(mpWireframeRS);
	//shaders
	m_pImmediateContext->VSSetShader(mpVS, nullptr, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, NumConstantBuffers, mpConstantBuffers);
	m_pImmediateContext->PSSetShader(mpPS, nullptr, 0);
	m_pImmediateContext->PSSetConstantBuffers(0, PSNumConstantBuffers, mpConstP);


	ObjectMatrices objMatx;

	//--------------------------BLEND STATE------------------
	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	//------------------------SKYSPHERE-----------------------
	// SET THE SKY SHADER
	m_pImmediateContext->PSSetShader(mpSkyPS, 0, 0);
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpSphereVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpSphereIB, DXGI_FORMAT_R32_UINT, 0);

	//constant buffers of the vertex shader
	objMatx.worldMatrix = XMLoadFloat4x4(&mSkyWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	m_pImmediateContext->RSSetState(RenderStates::NoCullRS);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkyMat, 0, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &mpSRV_SphereCubeMap);
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);



	//RETURN TO NORMAL SHADER
	

	if (key_ChangeShader)
	{
		m_pImmediateContext->PSSetShader(mpNewShader, 0, 0);

		
	}

	else {
		m_pImmediateContext->PSSetShader(mpPS, nullptr, 0);
	}
	//------------------------NORMAL SPHERE-----------------------
	//if (drawCenterSphere){
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpSphereVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpSphereIB, DXGI_FORMAT_R32_UINT, 0);

	//constant buffers of the vertex shader
	objMatx.worldMatrix = XMLoadFloat4x4(&mSphereWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	ID3D11ShaderResourceView *skyTexturesSRV[] = { mpSRV_FloorTex, nullptr, mpSRV_SphereCubeMap };
	//ID3D11ShaderResourceView *skyTexturesSRV[] = { mpSRV_FloorTex, mpDynamicCubeMapSRV };

	m_pImmediateContext->PSSetShaderResources(0, 3, skyTexturesSRV);
	m_pImmediateContext->RSSetState(0);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSphereMat, 0, 0);
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);
	//}


	//------------------------AIRBOX-----------------------

	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpAirBoxVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpAirBoxIB, DXGI_FORMAT_R32_UINT, 0);

	//constant buffers of the vertex shader
	objMatx.worldMatrix = XMLoadFloat4x4(&mAirBoxWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	ID3D11ShaderResourceView *airBoxTexturesSRV[] = { mpSRV_AirBoxOneMap, mpSRV_AirBoxTexture2 };
	//ID3D11ShaderResourceView *skyTexturesSRV[] = { mpSRV_FloorTex, mpDynamicCubeMapSRV };

	mAirBoxMat.FusionAlpha += change_airbox;
	if (mAirBoxMat.FusionAlpha >= 1 || mAirBoxMat.FusionAlpha <=0)
	{
		change_airbox *= -1;
	}
	m_pImmediateContext->PSSetShaderResources(0, 2, airBoxTexturesSRV);
	m_pImmediateContext->RSSetState(0);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mAirBoxMat, 0, 0);
	m_pImmediateContext->DrawIndexed(mAirBoxIndexCount, 0, 0);
	//}

	// GATO

	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpSnakeVB, &stride, &offset);

	//m_pImmediateContext->OMSetBlendState(mpTransparentBS, blendFactor, 0xffffffff);
	//constant buffers of the vertex shader
	objMatx.worldMatrix = XMLoadFloat4x4(&mSnakeWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	ID3D11ShaderResourceView * textureSnakeArray[] = { mpSRV_Snake, mpSRV_SphereCubeMap };
	//ID3D11ShaderResourceView * textureSkullArray[] = { mpSRV_floor, mpSRV_cubeMap };

	m_pImmediateContext->PSSetShaderResources(0, 2, textureSnakeArray);

	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSnakeMat, 0, 0);

	m_pImmediateContext->Draw(mSnakeVertexCount, 0);





	//---------------CAR-------------------------//
	// Car
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpCarVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpCarIB, DXGI_FORMAT_R32_UINT, 0);

	//constant buffers of the vertex shader
	objMatx.worldMatrix = XMLoadFloat4x4(&mCarWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	// Update the pixel Shader : Material Info
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mCarmat, 0, 0);

	m_pImmediateContext->DrawIndexed(mCarIndexCount, 0, 0);


	// TEAPOT
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpTeapotVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpTeapotIB, DXGI_FORMAT_R32_UINT, 0);

	//constant buffers of the vertex shader
	objMatx.worldMatrix = XMLoadFloat4x4(&mTeapotWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	
	m_pImmediateContext->PSSetShaderResources(0, 1, &mpSRV_Teapot);
	
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	// Update the pixel Shader : Material Info
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mTeapotmat, 0, 0);

	m_pImmediateContext->DrawIndexed(mTeapotIndexCount, 0, 0);

	// WAVES
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mWavesVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mWavesIB, DXGI_FORMAT_R32_UINT, 0);

	//ID3D11ShaderResourceView* arregloDeShaders[] = { mpSRV_Water, mpSRV_Water2 };
	objMatx.worldMatrix = XMLoadFloat4x4(&mWavesWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//ID3D11ShaderResourceView* arregloDeShaders[] = { mpSRV_Water, mpSRV_Water };

	m_pImmediateContext->PSSetShaderResources(0, 1, &mpSRV_Water);
	//m_pImmediateContext->RSSetState(mpWireframeRS);
	m_pImmediateContext->RSSetState(0);

	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mWavesMat, 0, 0);
	m_pImmediateContext->DrawIndexed(3 * mWaves.TriangleCount(), 0, 0);


	////-----------------------ROOM-------------------------
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpRoomVB, &stride, &offset);
	////constant buffers of the vertex shader

	m_pImmediateContext->RSSetState(RenderStates::NoCullRS);
	objMatx.worldMatrix = XMLoadFloat4x4(&mRoomWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mRoomMat, 0, 0);
	//m_pImmediateContext->PSSetShaderResources(0, 1 * enable_texture, &mpSRV_Wall);

	m_pImmediateContext->PSSetShaderResources(0, 1 * enable_texture, &mpSRV_Boxer_Array[increment_frame]);


	static float t_base4 = 0.1f;

	if ((mTimer.TotalTime() - t_base4) >= 3.0f)
	{
		t_base4 += 1.0f;
		increment_frame++;
		m_pImmediateContext->PSSetSamplers(0, 1, &mpSamplerState);
		m_pImmediateContext->PSSetShaderResources(0, 1 * enable_texture, &mpSRV_Boxer_Array[increment_frame]);

	}

	if (increment_frame == 55)
	{
		increment_frame = 0;

	}



	m_pImmediateContext->Draw(18,6);
	m_pImmediateContext->RSSetState(nullptr);
	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	////***********************

	//-----------------------SKULL-------------------------
	if (drawCenterSphere){
		m_pImmediateContext->IASetVertexBuffers(0, 1, &mpModelVB, &stride, &offset);
		m_pImmediateContext->IASetIndexBuffer(mpSkullIB, DXGI_FORMAT_R32_UINT, 0);
		//constant buffers of the vertex shader
		objMatx.worldMatrix = XMLoadFloat4x4(&mSkullWorld);
		objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
		objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
		objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//m_pImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
		//ID3D11ShaderResourceView *skullTexturesSRV[] = { mpSRV_FloorTex, mpSRV_SphereCubeMap };
		ID3D11ShaderResourceView *skullTexturesSRV[] = { mpSRV_FloorTex, nullptr,mpDynamicCubeMapSRV };
		m_pImmediateContext->PSSetShaderResources(0, 3*enable_texture , skullTexturesSRV);
		m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
		m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkullMat, 0, 0);
		m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);
		m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	}
	//-------------------------END SKULL-----------------------

	///----------------------DRAW MIRROR STENCIL---------------

	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpRoomVB, &stride, &offset);
	//m_pImmediateContext->IASetIndexBuffer(mpSkullIB, DXGI_FORMAT_R32_UINT, 0);

	//m_pImmediateContext->OMSetBlendState(mpTransparentBS, blendFactor, 0xffffffff);
	//constant buffers of the vertex shader
	objMatx.worldMatrix = XMLoadFloat4x4(&mRoomWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);

	//m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mMirrorMat, 0, 0);

	m_pImmediateContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactor, 0xffffffff);
	m_pImmediateContext->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);


	m_pImmediateContext->Draw(6, 24);
	m_pImmediateContext->OMSetDepthStencilState(0, 0);

	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	//--------------------SKULL REFLECTION---------------------------------

	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpModelVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpSkullIB, DXGI_FORMAT_R32_UINT, 0);

	//m_pImmediateContext->OMSetBlendState(mpTransparentBS, blendFactor, 0xffffffff);
	//constant buffers of the vertex shader
	XMVECTOR mirrorPlane = XMVectorSet(0, 0, 1.0f, 0.0);
	XMMATRIX R = XMMatrixReflect(mirrorPlane);

	objMatx.worldMatrix = XMLoadFloat4x4(&mSkullWorld)*R;
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkullMat, 0, 0);

	m_pImmediateContext->OMSetDepthStencilState(RenderStates::DrawReflectionDSS, 1);




	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);
	m_pImmediateContext->OMSetDepthStencilState(0, 0);

	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	//--------------------------DRAW MIRROR To THE BACK BUFFER ---------------------

	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpRoomVB, &stride, &offset);
	//m_pImmediateContext->IASetIndexBuffer(mpSkullIB, DXGI_FORMAT_R32_UINT, 0);

	//m_pImmediateContext->OMSetBlendState(mpTransparentBS, blendFactor, 0xffffffff);
	//constant buffers of the vertex shader
	objMatx.worldMatrix = XMLoadFloat4x4(&mRoomWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mMirrorMat, 0, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &mpSRV_mirror);

	m_pImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
	//m_pImmediateContext->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);


	m_pImmediateContext->Draw(6, 24);
	//m_pImmediateContext->OMSetDepthStencilState(0, 0);

	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);





	// ++++++++++++++++++++++ START SHAPES +++++++++++++++++++++++++
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

	// ------------------------ GRID ----------------------------
	objMatx.worldMatrix = XMLoadFloat4x4(&mShapeGridWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	ID3D11ShaderResourceView *gridTexturesSRV[] = { mpSRV_FloorTex,nullptr, mpSRV_SphereCubeMap };
	m_pImmediateContext->PSSetShaderResources(0, 3, gridTexturesSRV);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mShapeGridMat, 0, 0);
	m_pImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);
	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	// -------------------------------------------------------------
	
	



	// ------------------------ BOX ----------------------------
	objMatx.worldMatrix = XMLoadFloat4x4(&mShapeBoxWorld);
	objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
	objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	ID3D11ShaderResourceView *boxTexturesSRV[] = { mpSRV_FloorTex, nullptr,mpSRV_SphereCubeMap };
	
	
	
	
	m_pImmediateContext->PSSetShaderResources(0, 3, boxTexturesSRV);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mShapeBoxMat, 0, 0);
	m_pImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);
	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	// -------------------------------------------------------------


	// ------------------------ SPHERES ----------------------------
	ID3D11ShaderResourceView *sphereTexturesSRV[] = { mpSRV_GrassTex, nullptr, mpSRV_SphereCubeMap };
	for (int i = 0; i < 10; i++)
	{
		objMatx.worldMatrix = XMLoadFloat4x4(&mShapeSphereWorlds[i]);
		objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
		objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
		objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

		m_pImmediateContext->PSSetShaderResources(0, 3, sphereTexturesSRV);
		m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
		m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mShapeCylMat, 0, 0);
		m_pImmediateContext->DrawIndexed(mSphereIndexCount, mSphereIndexOffset, mSphereVertexOffset);
		m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	}

	m_pImmediateContext->RSSetState(mpWireframeRS);
	// -------------------------------------------------------------

	// ------------------------ CYLINDERS --------------------------
	ID3D11ShaderResourceView *cylinderTexturesSRV[] = { mpSRV_BricksTex, nullptr,mpSRV_SphereCubeMap };
	for (int i = 0; i < 10; i++)
	{
		objMatx.worldMatrix = XMLoadFloat4x4(&mShapeCylWorlds[i]);
		objMatx.wvp = objMatx.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
		objMatx.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatx.worldMatrix));
		objMatx.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);

		m_pImmediateContext->PSSetShaderResources(0, 3, cylinderTexturesSRV);
		m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatx, 0, 0);
		m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mShapeCylMat, 0, 0);
		m_pImmediateContext->DrawIndexed(mCylinderIndexCount, mCylinderIndexOffset, mCylinderVertexOffset);
		m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	}
	// -------------------------------------------------------------
	
}

void EnvMapApp::InitBuffers(){

	std::ifstream fin("Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, "skull.txt not found.", 0, 0);
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;  	// VertexCount: 31076
	fin >> ignore >> tcount;  	// TriangleCount: 60339
	fin >> ignore >> ignore >> ignore >> ignore; // VertexList (pos, normal)

	std::vector<VertexPNT> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
		vertices[i].Tex.x = sinf(textura_x + i);
		vertices[i].Tex.y = cosf(textura_y - i);
		
		//vertices[i].Tex.x = sinf(textura_x + i);
		//vertices[i].Tex.y = cosf(textura_y - i);
	}

	fin >> ignore; //}
	fin >> ignore; //triangleList
	fin >> ignore; //{

	mModelIndexCount = 3 * tcount;
	std::vector<UINT> indices(mModelIndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;             // we wont change this vertex
	bd.ByteWidth = sizeof(VertexPNT)* vcount;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      // use as a vertex buffer
	bd.CPUAccessFlags = 0;						  // we don't need to allow CPU to write in buffer
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA bdData;
	bdData.pSysMem = &vertices[0];

	HR(m_pDevice->CreateBuffer(&bd, &bdData, &mpModelVB));       // create the buffer

	//indices

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* mModelIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mpSkullIB));


	mObjReader = new ObjReader("Models/cat/cat.obj"); //nombre del archivo primero hice pruebas con una serpiente ahora con un gato
	std::vector<VertexPNT> verticesSnake;

	int modelSizes = mObjReader->model.size();
	std::vector<UINT> indicesSnake;
	int offset = 0;

	for (int m = 0; m < modelSizes; m++)
	{
		int nTri = mObjReader->model[m]->mesh->numTriangles;

		for (UINT i = 0; i < nTri; ++i)
		{
			for (int j = 0; j < 3; j++)
			{
				VertexPNT newV;
				Vec3f pos = mObjReader->model[m]->mesh->positions[mObjReader->model[m]->mesh->triangles[i * 3 + j]];
				Vec3f norm = mObjReader->model[m]->mesh->normals[mObjReader->model[m]->mesh->triangles[i * 3 + j]];
				Vec2f tex = mObjReader->model[m]->mesh->texcoords[mObjReader->model[m]->mesh->triangles[i * 3 + j]];
				newV.Pos = XMFLOAT3(pos.x, pos.y, pos.z);
				newV.Normal = XMFLOAT3(norm.x, norm.y, norm.z);
				newV.Tex = XMFLOAT2(tex.x, tex.y);
				verticesSnake.push_back(newV);
			}
		}

	}

	mSnakeIndexCount = indicesSnake.size();
	mSnakeVertexCount = verticesSnake.size();

	bd.ByteWidth = sizeof(VertexPNT)* verticesSnake.size();
	bdData.pSysMem = &verticesSnake[0];
	HR(m_pDevice->CreateBuffer(&bd, &bdData, &mpSnakeVB));

	mSnakeMat.Ambient = XMFLOAT4(1, 1, 1, 1.0f); //aqui pueden poner mObjReader->curMaterial->Ka.x ... si desean.
	mSnakeMat.Diffuse = XMFLOAT4(mObjReader->curMaterial->Kd.x, mObjReader->curMaterial->Kd.y, mObjReader->curMaterial->Kd.z, 1.0f);
	mSnakeMat.Specular = XMFLOAT4(mObjReader->curMaterial->Ks.x, mObjReader->curMaterial->Ks.y, mObjReader->curMaterial->Ks.z, 1.0f);
	mSnakeMat.Reflect = XMFLOAT4(0.4, 0.4, 0.4, 0.7);
	mSnakeMat.Refract = XMFLOAT4(0.4, 0.4, 0.4, mObjReader->curMaterial->Ni);
	mSnakeMat.EnableRefraction = 0;
	mSnakeMat.EnableReflection = 0;
	mSnakeMat.SpecularPower = mObjReader->curMaterial->Ns;
	mSnakeMat.UseTexture = 1;



}

void EnvMapApp::InitConstantBuffers(){
	// Create the constant buffers for the variables defined in the vertex shader.
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(ObjectMatrices);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstantBuffers[CB_Object]));

	constantBufferDesc.ByteWidth = sizeof(EYE);
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_EyeInfo]));

	constantBufferDesc.ByteWidth = sizeof(Light)*6;
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_LightInfo]));

	constantBufferDesc.ByteWidth = sizeof(ComplexMaterial);
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_MaterialInfo]));

	constantBufferDesc.ByteWidth = sizeof(Fog);
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_FogInfo]));
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_FogInfo], 0, nullptr, &mFog, 0, 0);


}

void EnvMapApp::InitPrecompiledShaders()
{
	ID3DBlob* pVS, *pPS, *pSkyPS, *pNewShader;

#if _DEBUG
	LPCWSTR compiledVShader = L"Debug/ShaderEMapV.cso";
	LPCWSTR compiledPShader = L"Debug/ShaderEMapP.cso";
	LPCWSTR compiledSkyPShader = L"Debug/ShaderSkyPd.cso";
	LPCWSTR compiledNewPShader = L"Debug/Shader2Pd.cso";
#else
	LPCWSTR compiledVShader = L"Release/ShaderEMapV.cso";
	LPCWSTR compiledPShader = L"Release/ShaderEMapP.cso";
	LPCWSTR compiledSkyPShader = L"Debug/ShaderSkyP.cso";
	LPCWSTR compiledNewPShader = L"Debug/Shader2P.cso";
#endif

	HR(D3DReadFileToBlob(compiledVShader, &pVS));
	HR(D3DReadFileToBlob(compiledPShader, &pPS));
	HR(D3DReadFileToBlob(compiledSkyPShader, &pSkyPS));
	HR(D3DReadFileToBlob(compiledNewPShader, &pNewShader));

	m_pDevice->CreateVertexShader(pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&mpVS);

	m_pDevice->CreatePixelShader(pPS->GetBufferPointer(),
		pPS->GetBufferSize(),
		nullptr,
		&mpPS);

	m_pDevice->CreatePixelShader(pSkyPS->GetBufferPointer(),
		pSkyPS->GetBufferSize(),
		nullptr,
		&mpSkyPS);

	m_pDevice->CreatePixelShader(pNewShader->GetBufferPointer(),
		pNewShader->GetBufferSize(),
		nullptr,
		&mpNewShader);

	m_pDevice->CreateInputLayout(iedPNT,
		ARRAYSIZE(iedPNT),
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		&mpLayout);

	Memory::SafeRelease(pVS);
	Memory::SafeRelease(pPS);
	Memory::SafeRelease(pSkyPS);
	Memory::SafeRelease(pNewShader);

}


void EnvMapApp::InitCarBuffers(){

	std::ifstream fin("Models/car.txt");

	if (!fin)
	{
		MessageBox(0, "car.txt not found.", 0, 0);
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;  	// VertexCount: 31076
	fin >> ignore >> tcount;  	// TriangleCount: 60339
	fin >> ignore >> ignore >> ignore >> ignore; // VertexList (pos, normal)

	std::vector<VertexPNT> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
	}

	fin >> ignore; //}
	fin >> ignore; //triangleList
	fin >> ignore; //{

	mCarIndexCount = 3 * tcount;
	std::vector<UINT> indices(mCarIndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;             // we wont change this vertex
	bd.ByteWidth = sizeof(VertexPNT)* vcount;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      // use as a vertex buffer
	bd.CPUAccessFlags = 0;						  // we don't need to allow CPU to write in buffer
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA bdData;
	bdData.pSysMem = &vertices[0];

	HR(m_pDevice->CreateBuffer(&bd, &bdData, &mpCarVB));       // create the buffer

	//indices

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* mCarIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mpCarIB));


}

void EnvMapApp::InitTextures()
{
	EffectFactory* pEffect = new EffectFactory(m_pDevice);

	pEffect->CreateTexture(L"Textures\\floor.dds",
		m_pImmediateContext,
		&mpSRV_FloorTex);
	pEffect->CreateTexture(L"Textures\\grass.dds",
		m_pImmediateContext,
		&mpSRV_GrassTex);
	pEffect->CreateTexture(L"Textures\\bricks.dds",
		m_pImmediateContext,
		&mpSRV_BricksTex);
	pEffect->CreateTexture(L"Textures\\floor_nmap.dds",
		m_pImmediateContext,
		&mpSRV_FloorTexNMap);

	pEffect->CreateTexture(L"Textures\\stones.dds",
		m_pImmediateContext,
		&mpSRV_AirBoxOneMap);
	pEffect->CreateTexture(L"Textures\\water1.dds",
		m_pImmediateContext,
		&mpSRV_AirBoxTexture2);

	pEffect->CreateTexture(L"Textures\\grasscube1024.dds",
		m_pImmediateContext,
		&mpSRV_SphereCubeMap);

	// TEapot
	
	pEffect->CreateTexture(L"Textures\\water1.dds",
		m_pImmediateContext,
		&mpSRV_Teapot);

	//Water
	pEffect->CreateTexture(L"Textures\\water1.dds", m_pImmediateContext,
		&mpSRV_Water);

	pEffect->CreateTexture(L"Textures\\water2.dds", m_pImmediateContext,
		&mpSRV_Water2);
	// Estructrua de la Calaca

	pEffect->CreateTexture(L"Textures\\calaca.dds", m_pImmediateContext,
		&mpSRV_Skull);


	//GATO

	pEffect->CreateTexture(L"Models\\cat\\cat_diff.jpg",
		m_pImmediateContext,
		&mpSRV_Snake);


	//Mirror
	
		pEffect->CreateTexture(L"Textures\\water1.dds",
		m_pImmediateContext,
		&mpSRV_Wall);


		pEffect->CreateTexture(L"Textures\\water1.dds",
			m_pImmediateContext,
			&mpSRV_mirror);


		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt001.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[0]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt002.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[1]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt003.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[2]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt004.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[3]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt005.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[4]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt006.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[5]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt007.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[6]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt008.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[7]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt009.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[8]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt010.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[9]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt011.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[10]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt012.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[11]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt013.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[12]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt014.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[13]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt015.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[14]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt016.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[15]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt017.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[16]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt018.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[17]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt019.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[18]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt020.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[19]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt021.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[20]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt022.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[21]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt023.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[22]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt024.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[23]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt025.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[24]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt026.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[25]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt027.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[26]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt028.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[27]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt029.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[28]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt030.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[29]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt031.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[30]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt032.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[31]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt033.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[32]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt034.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[33]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt035.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[34]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt036.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[35]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt037.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[36]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt038.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[37]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt039.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[38]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt040.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[39]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt041.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[40]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt042.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[41]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt043.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[42]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt044.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[43]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt045.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[44]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt046.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[45]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt047.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[46]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt048.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[47]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt049.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[48]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt050.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[49]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt051.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[50]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt052.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[51]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt053.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[52]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt054.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[53]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt055.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[54]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt056.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[55]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt057.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[56]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt058.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[57]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt059.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[58]);
		pEffect->CreateTexture(L"Textures\\BoltAnim\\Bolt060.bmp", m_pImmediateContext,
			&mpSRV_Boxer_Array[59]);


	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	HR(m_pDevice->CreateSamplerState(&samplerDesc, &mpSamplerState));
}

void EnvMapApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(m_hAppWnd);
	}
}

void EnvMapApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mCam.Pitch(dy);
		mCam.RotateY(dx);

	}


	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void EnvMapApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) == 0)
	{

	}
	ReleaseCapture();

}

void EnvMapApp::InitRoomBuffer()
{


	// Create and specify geometry.  For this sample we draw a floor
	// and a wall with a mirror on it.  We put the floor, wall, and
	// mirror geometry in one vertex buffer.
	//
	//   |--------------|
	//   |              |
	//   |----|----|----|
	//   |Wall|Mirr|Wall|
	//   |    | or |    |
	//   /--------------/
	//  /   Floor      /
	// /--------------/


	VertexPNT v[30];

	v[0] = VertexPNT(-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	v[1] = VertexPNT(-3.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = VertexPNT(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);

	v[3] = VertexPNT(-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	v[4] = VertexPNT(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);
	v[5] = VertexPNT(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f);

	// Wall: Observe we tile texture coordinates, and that we
	// leave a gap in the middle for the mirror.
	v[6] = VertexPNT(-3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[7] = VertexPNT(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = VertexPNT(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f);

	v[9] = VertexPNT(-3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[10] = VertexPNT(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f);
	v[11] = VertexPNT(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 2.0f);

	v[12] = VertexPNT(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[13] = VertexPNT(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = VertexPNT(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);

	v[15] = VertexPNT(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[16] = VertexPNT(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);
	v[17] = VertexPNT(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 2.0f);

	v[18] = VertexPNT(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = VertexPNT(-3.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = VertexPNT(7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);

	v[21] = VertexPNT(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = VertexPNT(7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);
	v[23] = VertexPNT(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 1.0f);

	// Mirror
	v[24] = VertexPNT(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[25] = VertexPNT(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[26] = VertexPNT(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[27] = VertexPNT(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[28] = VertexPNT(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[29] = VertexPNT(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexPNT) * 30;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &mpRoomVB));


}

void EnvMapApp::InitSpheres()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData sphere;

	geoGen.CreateSphere(0.5, 30, 30, sphere);
	int size = sphere.Vertices.size();
	std::vector<VertexPNT> vertices(size);
	for (int i = 0; i < size; i++)
	{
		vertices[i].Pos = sphere.Vertices[i].Position;
		vertices[i].Normal = sphere.Vertices[i].Normal;
		vertices[i].Tex = sphere.Vertices[i].TexC;
	}

	mSmallSphereIndexCount = sphere.Indices.size();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexPNT) * size;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &mpSphereVB));


	vbd.ByteWidth = sizeof(UINT) * mSmallSphereIndexCount;
	vbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vinitData.pSysMem = &sphere.Indices[0];
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &mpSphereIB));
}

void EnvMapApp::BuildDynamicCubeMapViews()
{
	// 
	// Cubemap is a special texture array with 6 elements. 
	// 
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = CubeMapSize;
	texDesc.Height = CubeMapSize;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 6;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
	ID3D11Texture2D* cubeTex = 0;
	HR(m_pDevice->CreateTexture2D(&texDesc, 0, &cubeTex));
	// 
	// Create a render target view to each cube map face 
	// (i.e., each element in the texture array). 
	// 
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.ArraySize = 1;
	rtvDesc.Texture2DArray.MipSlice = 0;
	for (int i = 0; i < 6; ++i)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		HR(m_pDevice->CreateRenderTargetView(cubeTex, &rtvDesc, &mpDynamicCubeMapRTV[i]));
	}
	// 
	// Create a shader resource view to the cube map. 
	// 
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;
	HR(m_pDevice->CreateShaderResourceView(cubeTex, &srvDesc, &mpDynamicCubeMapSRV));
	Memory::SafeRelease(cubeTex);
	// 
	// We need a depth texture for rendering the scene into the cubemap 
	// that has the same resolution as the cubemap faces. 
	// 
	D3D11_TEXTURE2D_DESC depthTexDesc;
	depthTexDesc.Width = CubeMapSize;
	depthTexDesc.Height = CubeMapSize;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.SampleDesc.Quality = 0;
	depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	ID3D11Texture2D* depthTex = 0;
	HR(m_pDevice->CreateTexture2D(&depthTexDesc, 0, &depthTex));
	// Create the depth stencil view for the entire cube 
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = depthTexDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	HR(m_pDevice->CreateDepthStencilView(depthTex, &dsvDesc, &mpDynamicCubeMapDSV));
	Memory::SafeRelease(depthTex);
	// 
	// Viewport for drawing into cubemap. 
	// 
	mCubeMapViewport.TopLeftX = 0.0f;
	mCubeMapViewport.TopLeftY = 0.0f;
	mCubeMapViewport.Width = (float)CubeMapSize;
	mCubeMapViewport.Height = (float)CubeMapSize;
	mCubeMapViewport.MinDepth = 0.0f;
	mCubeMapViewport.MaxDepth = 1.0f;

}

void EnvMapApp::BuildCubeFaceCamera(float x, float y, float z)
{
	// Generate the cube map about the given position. 
	XMFLOAT3 center(x, y, z);
	XMFLOAT3 worldUp(0.0f, 1.0f, 0.0f);
	// Look along each coordinate axis. 
	XMFLOAT3 targets[6] =
	{
		XMFLOAT3(x + 1.0f, y, z), // +X 
		XMFLOAT3(x - 1.0f, y, z), // -X 
		XMFLOAT3(x, y + 1.0f, z), // +Y 
		XMFLOAT3(x, y - 1.0f, z), // -Y 
		XMFLOAT3(x, y, z + 1.0f), // +Z 
		XMFLOAT3(x, y, z - 1.0f) // -Z 
	};
	// Use world up vector (0,1,0) for all directions except +Y/-Y. In these cases, we 
	// are looking down +Y or -Y, so we need a different "up" vector. 
	XMFLOAT3 ups[6] =
	{
		XMFLOAT3(0.0f, 1.0f, 0.0f), // +X 
		XMFLOAT3(0.0f, 1.0f, 0.0f), // -X 
		XMFLOAT3(0.0f, 0.0f, -1.0f), // +Y 
		XMFLOAT3(0.0f, 0.0f, +1.0f), // -Y 
		XMFLOAT3(0.0f, 1.0f, 0.0f), // +Z 
		XMFLOAT3(0.0f, 1.0f, 0.0f) // -Z 
	};
	for (int i = 0; i < 6; ++i)
	{
		mCubeMapCamera[i].LookAt(center, targets[i], ups[i]);
		mCubeMapCamera[i].SetLens(0.5f*XM_PI, 1.0f, 0.1f, 1000.0f);
		mCubeMapCamera[i].UpdateViewMatrix();
	}
}

void EnvMapApp::BuildShapeGeometryBuffers()
{
	GeometryGenerator::MeshData box;
	GeometryGenerator::MeshData grid;
	GeometryGenerator::MeshData sphere;
	GeometryGenerator::MeshData cylinder;

	GeometryGenerator geoGen;
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);
	geoGen.CreateGrid(20.0f, 30.0f, 50, 40, grid);
	geoGen.CreateSphere(0.5f, 20, 20, sphere);
	geoGen.CreateCylinder(0.5f, 0.5f, 3.0f, 15, 15, cylinder);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mBoxVertexOffset = 0;
	mGridVertexOffset = box.Vertices.size();
	mSphereVertexOffset = mGridVertexOffset + grid.Vertices.size();
	mCylinderVertexOffset = mSphereVertexOffset + sphere.Vertices.size();

	// Cache the index count of each object.
	mBoxIndexCount = box.Indices.size();
	mGridIndexCount = grid.Indices.size();
	mSphereIndexCount = sphere.Indices.size();
	mCylinderIndexCount = cylinder.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	mBoxIndexOffset = 0;
	mGridIndexOffset = mBoxIndexCount;
	mSphereIndexOffset = mGridIndexOffset + mGridIndexCount;
	mCylinderIndexOffset = mSphereIndexOffset + mSphereIndexCount;

	UINT totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size();

	UINT totalIndexCount =
		mBoxIndexCount +
		mGridIndexCount +
		mSphereIndexCount +
		mCylinderIndexCount;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<VertexPNT> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].Tex = box.Vertices[i].TexC;
		//vertices[k].TangentU = box.Vertices[i].TangentU;
	}

	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[k].Tex = grid.Vertices[i].TexC;
		//vertices[k].TangentU = grid.Vertices[i].TangentU;
	}

	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
		vertices[k].Tex = sphere.Vertices[i].TexC;
		//vertices[k].TangentU = sphere.Vertices[i].TangentU;
	}

	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
		vertices[k].Tex = cylinder.Vertices[i].TexC;
		//vertices[k].TangentU = cylinder.Vertices[i].TangentU;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexPNT) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &mShapesVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());
	indices.insert(indices.end(), cylinder.Indices.begin(), cylinder.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mShapesIB));
}

void EnvMapApp::InitAirBoxBuffers()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box	;

	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);
	int size = box.Vertices.size();
	std::vector<VertexPNT> vertices(size);
	for (int i = 0; i < size; i++)
	{
		vertices[i].Pos = box.Vertices[i].Position;
		vertices[i].Normal = box.Vertices[i].Normal;
		vertices[i].Tex = box.Vertices[i].TexC;
	}

	mAirBoxIndexCount = box.Indices.size();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexPNT) * size;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &mpAirBoxVB));


	vbd.ByteWidth = sizeof(UINT) * mAirBoxIndexCount;
	vbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vinitData.pSysMem = &box.Indices[0];
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &mpAirBoxIB));


}

void EnvMapApp::InitTeapotBuffers()
{
	std::ifstream fin("Models/teapot.txt");

	if (!fin)
	{
		MessageBox(0, "teapot.txt not found.", 0, 0);
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;  	// VertexCount: 31076
	fin >> ignore >> tcount;  	// TriangleCount: 60339
	fin >> ignore >> ignore >> ignore >> ignore; // VertexList (pos, normal)

	std::vector<VertexPNT> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
	}

	fin >> ignore; //}
	fin >> ignore; //triangleList
	fin >> ignore; //{

	mTeapotIndexCount = 3 * tcount;
	std::vector<UINT> indices(mTeapotIndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;             // we wont change this vertex
	bd.ByteWidth = sizeof(VertexPNT)* vcount;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      // use as a vertex buffer
	bd.CPUAccessFlags = 0;						  // we don't need to allow CPU to write in buffer
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA bdData;
	bdData.pSysMem = &vertices[0];

	HR(m_pDevice->CreateBuffer(&bd, &bdData, &mpTeapotVB));       // create the buffer

	//indices

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* mTeapotIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mpTeapotIB));




}

void EnvMapApp::InitWaveBuffers()
{
	mWaves.Init(160, 160, 0.5f, 0.03f, 3.25f, 0.4f);
	// Create the vertex buffer. Note that we allocate space only, as
	// we will be updating the data every time step of the simulation.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexPNT)* mWaves.VertexCount();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	HR(m_pDevice->CreateBuffer(&vbd, 0, &mWavesVB));
	// Create the index buffer. The index buffer is fixed, so we only
	// need to create and set once.
	std::vector<UINT> indices(3 * mWaves.TriangleCount()); // 3 indices per face
	// Iterate over each quad.
	UINT m = mWaves.RowCount();
	UINT n = mWaves.ColumnCount();
	int k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i*n + j;
			indices[k + 1] = i*n + j + 1;
			indices[k + 2] = (i + 1)*n + j;
			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i*n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;
			k += 6; // next quad
		}
	}
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mWavesIB));


}

void EnvMapApp::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));

	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; // D3D11_FILL_SOLID;  //
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mpWireframeRS));
}



int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	EnvMapApp tApp(hInstance);
	if (!tApp.Init())
		return 1;


	return tApp.Run();
}
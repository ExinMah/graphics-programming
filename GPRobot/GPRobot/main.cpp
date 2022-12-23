#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define PI 3.1429												// Value of pi
# define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))		// To calculate array size

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Graphics Programming Robot"

/*
 * --------------------------------------------------------------------
 *								Variables
 * --------------------------------------------------------------------
*/

// Variables for projection transformation
float pTx = 0.0, pTy = 0.0, pTSpeed = 0.5;
float pRy = 0.0, pRSpeed = 1.0;

// Varaibles for views (Orthographic and Perspective)
bool isOrtho = true;
double oNear = -10.0, oFar = 10.0;
double pNear = 1.0, pFar = 20.0;

// Variables for texture bitmap
BITMAP bmp;
HBITMAP hBmp = NULL;

/*
 * --------------------------------------------------------------------
 *					Windows Procedure (Input Messages)
 * --------------------------------------------------------------------
*/
LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
 * --------------------------------------------------------------------
 *						Windows Initialization
 * --------------------------------------------------------------------
*/

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * --------------------------------------------------------------------
 *					Texture and Projection Group
 * -------------------------------------------------------------------- 
*/

GLuint loadTexture(LPCSTR textureFile)
{
	GLuint texture = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	HBITMAP hBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), textureFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBmp, sizeof(bmp), &bmp);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

	DeleteObject(hBmp);
	return texture;
}
void destroyTexture(GLuint textureArray[])
{
	for (int i = 0; i < my_sizeof(textureArray) / my_sizeof(textureArray[0]); i++)
	{
		glDeleteTextures(1, &textureArray[i]);
	}

	glDisable(GL_TEXTURE_2D);
}

void projection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glTranslatef(pTx, pTy, 0.0);
	glRotatef(pRy, 0.0, 1.0, 0.0);

	if (isOrtho) {
		glOrtho(-10.0, 10.0, -10.0, 10.0, oNear, oFar);
	}
	else {
		gluPerspective(20.0, 1.0 /* width / height of screen */, -1.0, 1.0);
		glFrustum(-10.0, 10.0, -10.0, 10.0, pNear, pFar);
	}
}

/*
 * --------------------------------------------------------------------
 *	Objects Drawing Group : TBA (Should make this another class ah XD)
 * --------------------------------------------------------------------
*/

/************************** DRAW 2D OBJECTS **************************/
// Draw Circle

/************************** DRAW 3D OBJECTS **************************/

// Draw Sphere
void DrawLineSphere(GLdouble radius)
{
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_LINE);
	gluSphere(sphere, radius, 30, 30);
	gluDeleteQuadric(sphere);
}
void DrawFillSphere(GLdouble radius)
{
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, radius, 30, 30);
	gluDeleteQuadric(sphere);
}

// Draw Cube - TD : Adjust the texture coordinates
void DrawFillCube(float size)
{
	glBegin(GL_QUADS);
	// Face 1 : Bottom face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);

	// Face 2 : Left Face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, size);

	// Face 3 : Front Face
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(1, 0);
	glVertex3f(size, size, size);
	glTexCoord2f(0, 0);
	glVertex3f(size, 0.0f, size);

	// Face 4 : Right Face
	glTexCoord2f(0, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(size, size, size);

	// Face 5 : Top Face
	glTexCoord2f(0, 1);
	glVertex3f(size, size, size);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(size, size, 0.0f);

	// Face 6 : Back Face
	glTexCoord2f(0, 1);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, size, 0.0f);
	glEnd();
}

// Draw Cone
void DrawFillCone( double radius, double height)
{
	GLUquadricObj* cone = NULL;
	cone = gluNewQuadric();

	gluQuadricDrawStyle(cone, GLU_FILL);
	gluCylinder(cone, radius, 0, height, 30, 30);
	gluDeleteQuadric(cone);
}

// Draw Cylinder
void DrawFillCylinder(double baseRadius, double topRadius, double height)
{
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, baseRadius, topRadius, height, 30, 30);
	gluDeleteQuadric(cylinder);
}

// Draw Pyramid
void DrawLinePyramid(float size)
{
	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(size / 2, size, size / 2);

	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size / 2, size, size / 2);


	glVertex3f(size, 0.0f, size);
	glVertex3f(size / 2, size, size / 2);

	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size / 2, size, size / 2);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}
void DrawFillPyramid(float size)
{
	glBegin(GL_QUADS);
	glTexCoord3f(0, 0, 1);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord3f(1, 0, 1);
	glVertex3f(size, 0.0f, size);
	glTexCoord3f(1, 0, 0);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord3f(0, 0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);

	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, size);
	glEnd();
}

/*
 * --------------------------------------------------------------------
 *							Robot Parts
 * --------------------------------------------------------------------
*/

void robotHead()
{

}

void robotBody()
{

}

void robotArm()
{

}

void robotLeg()
{

}

// TBA : Other stuff like weapons, etc.

/*
 * --------------------------------------------------------------------
 *								Robot
 * --------------------------------------------------------------------
*/

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	projection();

	// Draw Robot Here
}

/*
 * --------------------------------------------------------------------
 *								WinMain
 * --------------------------------------------------------------------
*/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------
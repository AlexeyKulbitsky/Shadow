#include "GLES20Driver.h"
#include "../GLContext/EGLContextManager.h"

using namespace sh;
using namespace video;

GLES20Driver::GLES20Driver(EGLContextManager* contextManager)
	:m_contextManager(contextManager)
{
}

bool GLES20Driver::Init()
{
	return false;
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::BeginRendering()
{
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::EndRendering()
{
	glFlush();
	m_contextManager->SwapBuffers();
}
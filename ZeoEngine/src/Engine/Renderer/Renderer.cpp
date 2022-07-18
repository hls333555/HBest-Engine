#include "ZEpch.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/RenderCommand.h"

namespace ZeoEngine {

	RendererData Renderer::s_Data;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::OnWindowResize(U32 width, U32 height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	RendererStats& Renderer::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer::ResetStats()
	{
		s_Data.Stats.Reset();
	}

}

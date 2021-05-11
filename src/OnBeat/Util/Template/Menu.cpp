#include <OnBeat/Util/Template/Menu.h>
#include <AppCore/Platform.h>
#include <Hazel/Renderer/Renderer2D.h>
#include <Hazel/Renderer/RenderCommand.h>

namespace ul = ultralight;

namespace OnBeat
{
	Menu::Menu(const std::string& document, App* MainApp, std::string layerName)
		: Layer(MainApp, layerName)
	{
		Config.resource_path = "assets/resources/";
		Config.use_gpu_renderer = false;
		Config.device_scale = 1.0;

		ul::Platform::instance().set_config(Config);
		ul::Platform::instance().set_font_loader(ul::GetPlatformFontLoader());
		ul::Platform::instance().set_file_system(ul::GetPlatformFileSystem("."));
		ul::Platform::instance().set_logger(ul::GetDefaultLogger("ul.log"));

		factory.reset(new GLTextureSurfaceFactory());
		ul::Platform::instance().set_surface_factory(factory.get());

		renderer = ul::Renderer::Create();

		auto& window = Hazel::Application::Get().GetWindow();
		view = renderer->CreateView(window.GetWidth(), window.GetHeight(), false, nullptr);

		LoadDocument("file:///" + ul::String(document.c_str()));
	}

	void Menu::LoadDocument(const ul::String& document)
	{
		view->LoadURL(document);
		view->Focus();
	}

	void Menu::OnUpdate(Hazel::Timestep ms)
	{
		{
			Hazel::RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			Hazel::RenderCommand::Clear();
		}

		{
			renderer->Update();
			renderer->Render();

			GLTextureSurface* textureSurface = static_cast<GLTextureSurface*>(view->surface());

			if (textureSurface)
			{

				uint32_t width = textureSurface->width();
				uint32_t height = textureSurface->height();
				GLuint textureId = textureSurface->GetTextureAndSyncIfNeeded();

				GLubyte* data = new GLubyte[width * height * 4];

				glGetTextureImage(textureId, 0, GL_RGBA, GL_UNSIGNED_BYTE, width * height * 4, data);

				Hazel::Ref<Hazel::Texture2D> texture = Hazel::Texture2D::Create(width, height);
				texture->SetData(data, width * height * 4);

				Hazel::Renderer2D::BeginScene(*CameraController);

				Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { width / 100.0f, height / -100.0f }, texture);
				Hazel::Renderer2D::EndScene();
				delete[] data;
			}
		}

		UpdateMenu(ms);
	}

	void Menu::OnEvent(Hazel::Event& e)
	{
		Hazel::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Hazel::MouseMovedEvent>(HZ_BIND_EVENT_FN(Menu::OnMouseMove));
		dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(Menu::ResizeView));
		Layer::OnEvent(e);
	}

	bool Menu::OnMouseMove(Hazel::MouseMovedEvent& e)
	{
		ul::MouseEvent evt;
		evt.type = ul::MouseEvent::kType_MouseMoved;
		evt.x = e.GetX();
		evt.y = e.GetY();

		view->FireMouseEvent(evt);
		return true;
	}

	bool Menu::ResizeView(Hazel::WindowResizeEvent& e)
	{
		view->Resize(e.GetWidth(), e.GetHeight());
		return true;
	}

	Menu::~Menu()
	{

	}
}
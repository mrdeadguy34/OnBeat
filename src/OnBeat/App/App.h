#pragma once
#include <OnBeat/Config/Config.h>
#include <OnBeat/Util/AudioPlayer/AudioPlayer.h>
#include <Hazel/Core/Application.h>

typedef struct GLFWwindow GLFWwindow;

namespace OnBeat
{
	class App : public Hazel::Application
	{

		public:
			App();
			~App();

			static App& Get() { return *instance; }

			AudioPlayer AudioPlayer;
			Config::Settings Settings;

			void RefreshSettings();

			void SetWindowIcon(const std::string& path);

			GLFWwindow* GetNativeWindow() const { return nativeWindow; }

		private:
			static App* instance;

			//Window resize funcs
			void SetFullScreen(int monitor);

			GLFWwindow* nativeWindow;
	};
}
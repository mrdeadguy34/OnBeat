#include <OnBeat/Config/Config.h>
#include <magic_enum.hpp>
#include <filesystem>
#include <fstream>
#include <algorithm>

using json = nlohmann::json;

namespace OnBeat
{
	namespace Config
	{
		//Setup onbeat settings from json file
		Settings::Settings(const char* path)
		{
			std::ifstream input(path);
			json config;
			input >> config;
			input.close();

			DisplayWidth = config["Display"][0];
			DisplayHeight = config["Display"][1];
			Fullscreen = config["Fullscreen"];

			for (auto& [key, value] : config["Input"].items())
			{
				auto inputKey = magic_enum::enum_cast<Keys>(std::string(value));
				if (inputKey.has_value())
				{
					Input[key] = inputKey.value();
				}
				else
				{
					//Todo error checking and defaulting
				}
			}

			CurrentSkinPath = std::filesystem::current_path().string()
				+ "/assets/user/skins/" + std::string(config["CurrentSkin"]);
			std::replace(CurrentSkinPath.begin(), CurrentSkinPath.end(), '\\', '/');
			CurrentSkin = Skin::AppSkin(CurrentSkinPath + "/Skin.json");

			Volume = config["Volume"];
		}

		void to_json(json& j, const Settings& s)
		{
			j["Resolution"] = std::string(s.DisplayWidth + "x" + s.DisplayHeight);
			j["Fullscreen"] = s.Fullscreen;
			j["SkinPath"] = s.CurrentSkinPath;
			j["Skin"] = s.CurrentSkin.SkinName;
			j["Volume"] = s.Volume * 100;
		}

		void from_json(const json& j, Settings& s)
		{
			std::string resString = j["Resolution"];
			s.DisplayWidth = std::stoi(resString.substr(0, resString.find("x", 0)).c_str());
			s.DisplayHeight = std::stoi(resString.substr(resString.find("x", 0) + 1, resString.length()).c_str());
			j.at("Fullscreen").get_to(s.Fullscreen);
			j.at("Skin").get_to(s.CurrentSkinPath);
			s.CurrentSkin = Skin::AppSkin(s.CurrentSkinPath.c_str());
			s.Volume = j["Volume"] / 100;
		}
	}
}
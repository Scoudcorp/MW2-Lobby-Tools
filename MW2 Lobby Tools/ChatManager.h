#pragma once
class ChatManager
{
public:
	static bool HandleChatMessage(gentity_t *from, gentity_t *to, int mode, const char *chatText);
	static std::vector<std::string> SplitMessage(const char *chatText);

	static bool IsCommand(std::string text);
	static std::string GetCommand(std::string command);

	static std::string SanitizeString(std::string text);

	static int GetInt(std::string token);

	static void ShowHelp(gentity_t *to);
	static void SendLine(gentity_t *to, std::string text);
};


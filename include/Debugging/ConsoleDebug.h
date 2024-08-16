#ifndef CONSOLE_DEBUG
#define CONSOLE_DEBUG

#include <string>

// Scoped enums to avoid naming conflicts
enum class TextColor
{
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
    BLACK = 30,
    BRIGHT_RED = 91,
    BRIGHT_GREEN = 92,
    BRIGHT_YELLOW = 93,
    BRIGHT_BLUE = 94,
    BRIGHT_MAGENTA = 95,
    BRIGHT_CYAN = 96,
    BRIGHT_WHITE = 97,
    BRIGHT_BLACK = 90
};

enum class BackgroundColor
{
    RED = 41,
    GREEN = 42,
    YELLOW = 43,
    BLUE = 44,
    MAGENTA = 45,
    CYAN = 46,
    WHITE = 47,
    BLACK = 40,
    BRIGHT_RED = 101,
    BRIGHT_GREEN = 102,
    BRIGHT_YELLOW = 103,
    BRIGHT_BLUE = 104,
    BRIGHT_MAGENTA = 105,
    BRIGHT_CYAN = 106,
    BRIGHT_WHITE = 107,
    BRIGHT_BLACK = 100
};

// ANSI escape codes
std::string BOLD(const std::string& text)      { return "\033[1m" + text + "\033[0m"; }
std::string ITALIC(const std::string& text)    { return "\033[3m" + text + "\033[0m"; }
std::string UNDERLINE(const std::string& text) { return "\033[4m" + text + "\033[0m"; }

std::string ColoredText(const std::string& text, TextColor color)
{
    return "\033[" + std::to_string(static_cast<int>(color)) + "m" + text + "\033[0m";
}

std::string ColoredBackgroundText(const std::string& text, BackgroundColor color)
{
    return "\033[" + std::to_string(static_cast<int>(color)) + "m" + text + "\033[0m";
}

// Special output
std::string ERROR(const std::string& text)       { return ColoredText(BOLD(text), TextColor::RED); }
std::string FATAL_ERROR(const std::string& text) { return ColoredBackgroundText(BOLD(text), BackgroundColor::RED); }
std::string WARNING(const std::string& text)     { return ColoredText(BOLD(text), TextColor::YELLOW); }

#endif // !CONSOLE_DEBUG
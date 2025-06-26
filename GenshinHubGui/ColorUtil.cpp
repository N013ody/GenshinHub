
#include "ColorUtil.h"


ImVec4 HexToImVec4(const std::string& hex, float alpha ) {
    std::string cleaned = hex;
    if (!cleaned.empty() && cleaned[0] == '#') {
        cleaned = cleaned.substr(1);
    }

  
    if (cleaned.length() == 3) {
        cleaned = std::string() +
            cleaned[0] + cleaned[0] +
            cleaned[1] + cleaned[1] +
            cleaned[2] + cleaned[2];
    }

    if (cleaned.length() != 6) return ImVec4(0, 0, 0, alpha);

    auto hexToFloat = [](const std::string& str) -> float {
        return std::stoi(str, nullptr, 16) / 255.0f;
        };

    float r = hexToFloat(cleaned.substr(0, 2));
    float g = hexToFloat(cleaned.substr(2, 2));
    float b = hexToFloat(cleaned.substr(4, 2));

    return ImVec4(r, g, b, alpha);
}

ImColor HexToImColor(const std::string& hex, float alpha) {
    return ImColor(HexToImVec4(hex, alpha));
}

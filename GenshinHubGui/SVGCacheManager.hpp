#pragma once
#include <DX11Util.h>
#include <unordered_map>
#include <nanosvgrast.h>
#include <TimeHelper.hpp>
#include <mutex>

class SVGCacheManager {
private:

    using QuantizedScale = int;

    QuantizedScale Quantize(float scale) const {
        return static_cast<int>(std::round(scale * 1000));
    }

    struct CachedTexture {
        SVGTexture texture;
        size_t lastUsedTime;
        float scale;
        bool IsValid() const { return texture.srv != nullptr; }
    };


    std::unordered_map<float, CachedTexture> _scaleCache;
    NSVGimage* _svgSource = nullptr;         //SVG解析结果缓存
    ID3D11Device* _d3dDevice;
    size_t _maxCacheSize = 5;                //最大缓存数

    std::mutex _cacheMutex;

public:

    void SetDevice(ID3D11Device* d3dDevice) {
        _d3dDevice = d3dDevice;
    }

    void SetMaxCacheSize(size_t size) { _maxCacheSize = size; }

    bool LoadSVG(const char* path,const char* uints="px", float dpi=96.f) {
        if (!_svgSource) {
            _svgSource = nsvgParseFromFile(path,uints,dpi);
            
        }
        return _svgSource != nullptr;
    }


    bool LoadSVGFromMemory(char* input, const char* uints = "px", float dpi = 96.f) {
        if (!_svgSource) {
            _svgSource = nsvgParse(input, uints, dpi);

        }
        return _svgSource != nullptr;
    }

    //获取带缓存的缩放纹理
    SVGTexture GetScaledTexture(float scale) {
        const auto key = Quantize(scale);
     
        //查找
        auto it = _scaleCache.find(key);
        if (it != _scaleCache.end()) {
            if (it->second.texture.srv!=nullptr) { 
                it->second.lastUsedTime = GetCurrentTimestamp();
                return it->second.texture;
            }
            else {
                _scaleCache.erase(it); //清理无效条目
            }

        }
        
        //创建纹理
        SVGTexture newTex = CreateSVGTexture(_d3dDevice, _svgSource, scale);

        //缓存清理
       
        if (_scaleCache.size() >= _maxCacheSize) {
            auto oldest = std::min_element(_scaleCache.begin(), _scaleCache.end(),
                [](auto& a, auto& b) { return a.second.lastUsedTime < b.second.lastUsedTime; });
            _scaleCache.erase(oldest);
        }
        
        _scaleCache.emplace(key, CachedTexture{ newTex, GetCurrentTimestamp(), scale });

        //存入缓存
        _scaleCache[scale] = { newTex, GetCurrentTimestamp(), scale };
        return newTex;
    }
};

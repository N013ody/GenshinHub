#include <EasingAnimationSystem.h>
using namespace ImEasing;
class ColorAnimator {
public:
    ColorAnimator(const ImVec4& initialColor, Params animParams)
        : currentColor(initialColor), lastTargetColor(initialColor),
        params(animParams) {
        anim = std::make_shared<Animation<ImVec4>>();
    }

    void UpdateTarget(const ImVec4& newTarget) {
        if (!ImVec4Equal(newTarget, lastTargetColor)) {
            anim->Start(currentColor, newTarget, params);
            lastTargetColor = newTarget;
        }
    }

    void Tick() {
        anim->Update();
        currentColor = (anim->GetState() == ImEasing::Running)
            ? anim->GetCurrentValue()
            : lastTargetColor;
    }

    const ImVec4& Current() const { return currentColor; }

private:
    bool ImVec4Equal(const ImVec4& a, const ImVec4& b, float eps = 1e-5f) {
        return fabs(a.x - b.x) < eps && fabs(a.y - b.y) < eps &&
            fabs(a.z - b.z) < eps && fabs(a.w - b.w) < eps;
    }

    std::shared_ptr<Animation<ImVec4>> anim;
    ImVec4 currentColor;
    ImVec4 lastTargetColor;
    Params params;
};

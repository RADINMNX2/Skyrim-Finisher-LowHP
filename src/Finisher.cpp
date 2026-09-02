#include "Finisher.h"

#include "settings.h"

namespace {
    bool IsTargetEligible(RE::Actor* a_actor) {
        if (!a_actor) return false;
        if (a_actor->IsDead()) return false;
        if (a_actor->IsPlayerRef()) return false;

        // Only execute actors that are woundable / have a finite health pool.
        const float hp = a_actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
        return hp <= Settings::Get().finisherHP;
    }

    void Execute(RE::Actor* a_victim) {
        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) return;

        SKSE::log::info(
            "Executing '{}' (hp={})",
            a_victim->GetDisplayFullName(),
            a_victim->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth));

        // Kill the target, credited to the player. The game plays the normal
        // death/ragdoll. Killing a low-HP actor inside combat often lets the
        // engine pick a killmove on this final blow.
        a_victim->Kill(player);
    }

    class FinisherInputHandler : public RE::BSTEventSink<RE::InputEvent> {
    public:
        using EventResult = RE::BSEventNotifyControl;

        EventResult ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override {
            if (!a_event) return EventResult::kContinue;

            for (auto* ev = *a_event; ev; ev = ev->next) {
                if (ev->GetEventType() != RE::InputEvent::EventType::kButton) {
                    continue;
                }

                const auto* button = static_cast<const RE::ButtonEvent*>(ev);
                if (!button->IsDown()) {
                    continue;
                }

                const auto* key = ev->As<RE::KeyEvent>();
                if (!key || key->idCode != Settings::Get().finisherKey) {
                    continue;
                }

                // Don't execute while the game is paused (menus open).
                if (const auto* ui = RE::UI::GetSingleton(); ui && ui->GameIsPaused()) {
                    continue;
                }

                auto* player = RE::PlayerCharacter::GetSingleton();
                if (!player) {
                    continue;
                }

                // Weapon drawn check: only finisher an enemy we are fighting.
                // (Optional; skip if the game hasn't assigned the actor yet.)
                auto* crosshair = RE::CrosshairPickData::GetSingleton()->target.get();
                if (!crosshair) {
                    continue;
                }

                auto* actor = crosshair->As<RE::Actor>();
                if (!IsTargetEligible(actor)) {
                    continue;
                }

                Execute(actor);
            }

            return EventResult::kContinue;
        }
    };
} // namespace

namespace Finisher {
    void Install() {
        static FinisherInputHandler handler;

        auto* eventSource = RE::InputEventSource::GetEventSource();
        if (!eventSource) {
            SKSE::log::error("Finisher: failed to get input event source");
            return;
        }

        eventSource->AddEventSink(&handler);
        SKSE::log::info("Finisher: installed input sink (key={:#x})", Settings::Get().finisherKey);
    }
} // namespace Finisher

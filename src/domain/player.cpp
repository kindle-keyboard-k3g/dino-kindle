#include "domain/player.h"

namespace dino::domain {

void PlayerPhysics::apply_jump() {
    if (!is_on_ground()) {
        return;
    }
    velocity_y_ = FixedPoint::from_pixels(JUMP_IMPULSE_PIXELS);
}

void PlayerPhysics::update() {
    y_ = y_ + velocity_y_;
    velocity_y_ = velocity_y_ + FixedPoint::from_pixels(GRAVITY_PIXELS);
    const FixedPoint ground = FixedPoint::from_pixels(GROUND_Y);
    if (y_ >= ground) {
        y_ = ground;
        velocity_y_ = FixedPoint::from_pixels(0);
    }
}

void PlayerPhysics::reset() {
    y_ = FixedPoint::from_pixels(GROUND_Y);
    velocity_y_ = FixedPoint::from_pixels(0);
}

ScreenCoordinate Player::position() const {
    const int32_t current_y = physics_.y().to_pixels() - dimensions().height();
    return ScreenCoordinate(X_POSITION, current_y);
}

Dimensions Player::dimensions() const {
    if (animation_.state() == PlayerState::Ducking) {
        return Dimensions(DUCKING_WIDTH, DUCKING_HEIGHT);
    }
    return Dimensions(STANDING_WIDTH, STANDING_HEIGHT);
}

BoundingBox Player::visual_box() const {
    return BoundingBox(position(), dimensions());
}

BoundingBox Player::hitbox() const {
    return visual_box().inset(HITBOX_INSET);
}

void Player::jump() {
    if (animation_.state() == PlayerState::Crashed) {
        return;
    }
    if (!physics_.is_on_ground()) {
        return;
    }
    physics_.apply_jump();
    animation_.set_state(PlayerState::Jumping);
}

void Player::duck() {
    if (animation_.state() == PlayerState::Crashed) {
        return;
    }
    if (!physics_.is_on_ground()) {
        return;
    }
    animation_.set_state(PlayerState::Ducking);
}

void Player::stand() {
    if (animation_.state() == PlayerState::Crashed) {
        return;
    }
    if (!physics_.is_on_ground()) {
        return;
    }
    animation_.set_state(PlayerState::Running);
}

void Player::crash() {
    animation_.set_state(PlayerState::Crashed);
}

void Player::update() {
    if (animation_.state() == PlayerState::Crashed) {
        return;
    }
    physics_.update();
    animation_.advance_step();
    if (physics_.is_on_ground() && animation_.state() == PlayerState::Jumping) {
        animation_.set_state(PlayerState::Running);
    }
}

void Player::reset() {
    physics_.reset();
    animation_.reset();
}

}  // namespace dino::domain

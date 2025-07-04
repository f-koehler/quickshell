#pragma once
#include <qcontainerfwd.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qtypes.h>

#include "../../dbus/properties.hpp"
#include "dbus_kbd_backlight.h"

namespace qs::service::upower {

class UPowerKbdBacklight: public QObject {
	Q_OBJECT
	Q_PROPERTY(int brightness READ default NOTIFY brightnessChanged BINDABLE bindableBrightness)
	Q_PROPERTY(int maxBrightness READ default NOTIFY maxBrightnessChanged BINDABLE
	               bindableMaxBrightness)

public:
	explicit UPowerKbdBacklight(QObject* parent = nullptr);
	void init();
	void initBrightness();
	void initMaxBrightness();

	[[nodiscard]] QBindable<int> bindableBrightness() const { return &this->bBrightness; }
	[[nodiscard]] QBindable<int> bindableMaxBrightness() const { return &this->bMaxBrightness; }

signals:
	void brightnessChanged();
	void maxBrightnessChanged();

private:
	// clang-format off
	Q_OBJECT_BINDABLE_PROPERTY(UPowerKbdBacklight, int,	bBrightness, &UPowerKbdBacklight::brightnessChanged);
	Q_OBJECT_BINDABLE_PROPERTY(UPowerKbdBacklight, int, bMaxBrightness, &UPowerKbdBacklight::maxBrightnessChanged);
	// clang-format on

	DBusUPowerXkbBacklight* backlight = nullptr;
};

} // namespace qs::service::upower

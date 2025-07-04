#include "kbd_backlight.hpp"

#include "../../dbus/properties.hpp"

using namespace qs::dbus;

namespace qs::service::upower {

namespace {
Q_LOGGING_CATEGORY(logUPowerKbdBacklight, "quickshell.service.upower.kbd_backlight", QtWarningMsg);
}

UPowerKbdBacklight::UPowerKbdBacklight(QObject* parent): QObject(parent) {}

void UPowerKbdBacklight::init() {
	this->backlight = new DBusUPowerXkbBacklight(
	    "org.freedesktop.UPower",
	    "/org/freedesktop/UPower/KbdBacklight",
	    QDBusConnection::systemBus(),
	    this
	);

	if (!this->backlight->isValid()) {
		qCWarning(logUPowerKbdBacklight) << "Cannot create UPower.KbdBacklight";
		return;
	}

	QObject::connect(this->backlight, &DBusUPowerXkbBacklight::BrightnessChanged, this, [this]() {
		Q_EMIT(brightnessChanged());
	});
	QObject::connect(
	    this->backlight,
	    &DBusUPowerXkbBacklight::BrightnessChangedWithSource,
	    this,
	    [this]() { Q_EMIT(brightnessChanged()); }
	);

	initBrightness();
	initMaxBrightness();
}

void UPowerKbdBacklight::initBrightness() {
	if (!this->backlight || !this->backlight->isValid()) {
		bBrightness = 0;
		return;
	}
	auto pending = this->backlight->GetBrightness();
	auto* call = new QDBusPendingCallWatcher(pending, this);

	auto responseCallBack = [this](QDBusPendingCallWatcher* call) {
		const QDBusPendingReply<int> reply = *call;

		if (reply.isError()) {
			qCWarning(logUPowerKbdBacklight)
			    << "Failed to get initial brightness:" << reply.error().message();
		} else {
			qCDebug(logUPowerKbdBacklight)
			    << "Retrieved initial UPower keyboard brightness:" << reply.value();
			this->bBrightness.setValue(reply.value());
		}
		delete call;
	};

	QObject::connect(call, &QDBusPendingCallWatcher::finished, this, responseCallBack);
}

void UPowerKbdBacklight::initMaxBrightness() {
	if (!this->backlight || !this->backlight->isValid()) {
		bMaxBrightness = 1;
		return;
	}
	auto pending = this->backlight->GetMaxBrightness();
	auto* call = new QDBusPendingCallWatcher(pending, this);

	auto responseCallBack = [this](QDBusPendingCallWatcher* call) {
		const QDBusPendingReply<int> reply = *call;

		if (reply.isError()) {
			qCWarning(logUPowerKbdBacklight)
			    << "Failed to get initial max brightness:" << reply.error().message();
		} else {
			qCDebug(logUPowerKbdBacklight)
			    << "Retrieved initial UPower keyboard max brightness:" << reply.value();
			this->bMaxBrightness.setValue(reply.value());
		}
		delete call;
	};

	QObject::connect(call, &QDBusPendingCallWatcher::finished, this, responseCallBack);
}

} // namespace qs::service::upower

#pragma once
#include "legacy/legacy.h"

/*
	This file is providing fabricators for controls. It tries to guess control type by legacy names, then constructs and returns control.
*/
class abs_control;
class QBoxLayout;
abs_control* fabricateControl(InputControl control, QBoxLayout* layout, QWidget* parent);

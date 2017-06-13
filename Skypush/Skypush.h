#pragma once

#include <QtWidgets/QWidget>
#include "ui_Skypush.h"

class Skypush : public QWidget
{
	Q_OBJECT

public:
	Skypush(QWidget *parent = Q_NULLPTR);

private:
	Ui::SkypushClass ui;
};

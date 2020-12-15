/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout;
    GLWidget *widget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_2;
    QPushButton *SliceAuto;
    QCheckBox *checkBox;
    QDoubleSpinBox *SliceAdaptive;
    QPushButton *saveGCode;
    QPushButton *ResetButton;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(889, 585);
        horizontalLayout = new QHBoxLayout(Widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget = new GLWidget(Widget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(widget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        SliceAuto = new QPushButton(Widget);
        SliceAuto->setObjectName(QString::fromUtf8("SliceAuto"));

        verticalLayout->addWidget(SliceAuto);

        checkBox = new QCheckBox(Widget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout->addWidget(checkBox);

        SliceAdaptive = new QDoubleSpinBox(Widget);
        SliceAdaptive->setObjectName(QString::fromUtf8("SliceAdaptive"));
        SliceAdaptive->setMinimum(1.000000000000000);
        SliceAdaptive->setMaximum(5.000000000000000);
        SliceAdaptive->setSingleStep(0.050000000000000);
        SliceAdaptive->setValue(1.000000000000000);

        verticalLayout->addWidget(SliceAdaptive);

        saveGCode = new QPushButton(Widget);
        saveGCode->setObjectName(QString::fromUtf8("saveGCode"));

        verticalLayout->addWidget(saveGCode);

        ResetButton = new QPushButton(Widget);
        ResetButton->setObjectName(QString::fromUtf8("ResetButton"));

        verticalLayout->addWidget(ResetButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(Widget);
        QObject::connect(pushButton, SIGNAL(clicked()), Widget, SLOT(close()));

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Slicer TurboMaxHiTechProSpeedUpUltraHighMegaUltra Remastered HD Collection Championship Edition Featuring Dante & Knuckles from Devil May Cry series, with new Funky Mode", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Widget", "OpenFile", nullptr));
        SliceAuto->setText(QCoreApplication::translate("Widget", "Slice", nullptr));
        checkBox->setText(QCoreApplication::translate("Widget", "Wireframe", nullptr));
        saveGCode->setText(QCoreApplication::translate("Widget", "GCode", nullptr));
        ResetButton->setText(QCoreApplication::translate("Widget", "Reset", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "Quit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H

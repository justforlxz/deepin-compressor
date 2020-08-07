/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chendu <chendu@uniontech.com>
*
* Maintainer: chendu <chendu@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H
#include <DSuggestButton>
#include <QComboBox>

DWIDGET_USE_NAMESPACE
class CustomSuggestButton: public DSuggestButton
{
    Q_OBJECT
public:
    explicit CustomSuggestButton(QWidget *parent = nullptr);
    explicit CustomSuggestButton(const QString &text, QWidget *parent = nullptr);

protected:
//    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
//    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};

class CustomCombobox: public QComboBox
{
    Q_OBJECT
public:
    explicit CustomCombobox(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};

#endif // CUSTOMWIDGET_H
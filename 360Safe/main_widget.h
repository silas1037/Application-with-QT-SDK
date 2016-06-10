#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include "title_widget.h"
#include "content_widget.h"
#include "system_tray.h"
#include "about_us.h"
#include "main_menu.h"
#include "character_widget.h"
#include "setting_dialog.h"
#include "skin_widget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:

    MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:

    void paintEvent(QPaintEvent *);

public slots:

	void showWidget();

private slots:

	void showMax();
	void showSkinWidget();
	void showMainMenu();
	void showSettingDialog();
	void showAboutUs();
	void showNewCharacter();
	void changeSkin(QString skin_name);
	void iconIsActived(QSystemTrayIcon::ActivationReason reason);

private:

	QRect location;
	TitleWidget *title_widget; //标题栏
	ContentWidget *content_widget; //主界面内容
	SystemTray *system_tray; //托盘项
	SettingDialog *setting_dialog; //设置界面
	CharacterWidget *character_widget; //新版特性界面
	AboutUsDialog *about_us_dialog; //关于我们界面
    QString skin_name;//主窗口背景图片的名称
	MainMenu *main_menu; //主菜单
	SkinWidget *skin_widget; //显示皮肤界面

};

#endif // MAINWIDGET_H

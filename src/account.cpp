#include "account.h"
#include "ui_account.h"
#include "gui.h"
#include "skypush.h"
#include <QUrlQuery>
#include <QMessageBox>
#include <QNetworkReply>
#include <QPushButton>

Account::Account(GUI *parent) :
    ui(new Ui::Account),
    settingsManager(new QSettings())
{
    gui = parent;
    ui->setupUi(this);
    ui->loginButtonBox->button(QDialogButtonBox::Ok)->setText("Log in");
    ui->loggedInButtonBox->button(QDialogButtonBox::Ok)->setText("Log out");
    networkManager = new QNetworkAccessManager(this);
    ui->existingUserToggle->setCheckState(Qt::Checked);
    initialize();
}

Account::~Account()
{
    delete ui;
}

void Account::initialize()
{
    if (settingsManager->contains("sessionToken"))
    {
        enableLogin(false);
        ui->loggedInLabel->setText(QString("Logged in as %1").arg(settingsManager->value("sessionEmail").toString()));
    }
    else
    {
        enableLogin(true);
        ui->editEmail->setFocus();
    }
}

void Account::enableLogin(bool state)
{
    ui->loginWidget->setVisible(state);
    ui->loggedInWidget->setVisible(!state);
}

void Account::on_existingUserToggle_stateChanged(int state)
{
    if (state == 0)
    {
        ui->editVerifyPassword->setEnabled(true);
    }
    else
    {
        ui->editVerifyPassword->setEnabled(false);
    }
}

void Account::on_loginButtonBox_accepted()
{
    if (ui->existingUserToggle->checkState() == Qt::Checked)
    {
        login();
    }
    else
    {
        createUser();
    }
    return;
}

void Account::on_loginButtonBox_rejected()
{
    reject();
}

void Account::on_loggedInButtonBox_accepted()
{
    settingsManager->remove("accessToken");
    settingsManager->remove("sessionToken");
    settingsManager->remove("sessionEmail");
    initialize();
    return;
}

void Account::on_loggedInButtonBox_rejected()
{
    reject();
}

void Account::createUser()
{
    QUrlQuery postData;
    postData.addQueryItem("email", ui->editEmail->text());
    postData.addQueryItem("password", ui->editPassword->text());
    postData.addQueryItem("passwordVerify", ui->editVerifyPassword->text());
    QNetworkRequest request(QUrl("https://skyweb.nu/api/createUser.php"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply* reply = networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, SIGNAL(finished()), this, SLOT(createUserReply()));
}

void Account::createUserReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QJsonObject response = Skypush::jsonToObject(reply->readAll());
    if (reply->error() == QNetworkReply::NoError)
    {
        login();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "Creating new user failed", response["message"].toString());
    }
    reply->deleteLater();
}

void Account::login()
{
    QNetworkRequest request(QUrl("https://skyweb.nu/api/login.php"));
    QString token = settingsManager->value("clientToken").toString();
    request.setRawHeader("token", token.toUtf8());
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(ui->editEmail->text()).arg(ui->editPassword->text()).toLatin1()).toBase64());
    QByteArray empty;
    QNetworkReply* reply = networkManager->post(request, empty);
    connect(reply, SIGNAL(finished()), this, SLOT(loginReply()));
}

void Account::loginReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QJsonObject response = Skypush::jsonToObject(reply->readAll());
    if (reply->error() == QNetworkReply::NoError)
    {
        settingsManager->setValue("sessionToken", response["message"].toString());
        settingsManager->setValue("sessionEmail", ui->editEmail->text());
        gui->setAccessToken();
        initialize();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "Login failed", response["message"].toString());
    }

    reply->deleteLater();
}

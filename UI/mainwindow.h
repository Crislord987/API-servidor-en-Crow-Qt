#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QProgressBar>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QTimer>
#include <QTime>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // Widgets de interfaz
    QWidget* m_centralWidget;
    QVBoxLayout* m_mainLayout;

    // Sección de registro
    QGroupBox* m_registerGroup;
    QLineEdit* m_registerUsername;
    QLineEdit* m_registerPassword;
    QPushButton* m_registerButton;

    // Sección de login
    QGroupBox* m_loginGroup;
    QLineEdit* m_loginUsername;
    QLineEdit* m_loginPassword;
    QPushButton* m_loginButton;

    // Sección de estado
    QGroupBox* m_statusGroup;
    QLabel* m_statusLabel;
    QTextEdit* m_responseDisplay;
    QPushButton* m_viewUsersButton;
    QPushButton* m_clearLogButton;

    // Progress y estado
    QProgressBar* m_progressBar;
    QLabel* m_tokenLabel;

    // API
    QNetworkAccessManager* m_networkManager;
    QString m_baseUrl;
    QString m_currentToken;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRegisterClicked();
    void onLoginClicked();
    void onViewUsersClicked();
    void onClearLogClicked();
    void onRegisterFinished(QNetworkReply* reply);
    void onLoginFinished(QNetworkReply* reply);
    void onViewUsersFinished(QNetworkReply* reply);

private:
    void setupUI();
    void logMessage(const QString& message, const QString& color = "black");
    void showProgress(bool show);
    void updateTokenDisplay();
};

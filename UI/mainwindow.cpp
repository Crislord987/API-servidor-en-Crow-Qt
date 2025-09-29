#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_baseUrl("http://localhost:8080")
{
    setupUI();
    updateTokenDisplay();
    setWindowTitle("Sistema de Autenticación - Cliente Qt");
    setMinimumSize(600, 700);
    resize(800, 700);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_mainLayout = new QVBoxLayout(m_centralWidget);

    // ===== SECCIÓN DE REGISTRO =====
    m_registerGroup = new QGroupBox(" Registrar Usuario", this);
    QVBoxLayout* registerLayout = new QVBoxLayout(m_registerGroup);

    // Campos de registro
    QHBoxLayout* regUserLayout = new QHBoxLayout();
    regUserLayout->addWidget(new QLabel("Usuario:"));
    m_registerUsername = new QLineEdit();
    m_registerUsername->setPlaceholderText("Ingresa tu usuario");
    regUserLayout->addWidget(m_registerUsername);
    registerLayout->addLayout(regUserLayout);

    QHBoxLayout* regPassLayout = new QHBoxLayout();
    regPassLayout->addWidget(new QLabel("Password:"));
    m_registerPassword = new QLineEdit();
    m_registerPassword->setEchoMode(QLineEdit::Password);
    m_registerPassword->setPlaceholderText("Ingresa tu password");
    regPassLayout->addWidget(m_registerPassword);
    registerLayout->addLayout(regPassLayout);

    m_registerButton = new QPushButton("Registrar", this);
    m_registerButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px; }");
    registerLayout->addWidget(m_registerButton);

    m_mainLayout->addWidget(m_registerGroup);

    // ===== SECCIÓN DE LOGIN =====
    m_loginGroup = new QGroupBox("Iniciar Sesión", this);
    QVBoxLayout* loginLayout = new QVBoxLayout(m_loginGroup);

    // Campos de login
    QHBoxLayout* loginUserLayout = new QHBoxLayout();
    loginUserLayout->addWidget(new QLabel("Usuario:"));
    m_loginUsername = new QLineEdit();
    m_loginUsername->setPlaceholderText("Tu usuario registrado");
    loginUserLayout->addWidget(m_loginUsername);
    loginLayout->addLayout(loginUserLayout);

    QHBoxLayout* loginPassLayout = new QHBoxLayout();
    loginPassLayout->addWidget(new QLabel("Password:"));
    m_loginPassword = new QLineEdit();
    m_loginPassword->setEchoMode(QLineEdit::Password);
    m_loginPassword->setPlaceholderText("Tu password");
    loginPassLayout->addWidget(m_loginPassword);
    loginLayout->addLayout(loginPassLayout);

    m_loginButton = new QPushButton("Iniciar Sesión", this);
    m_loginButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; padding: 8px; }");
    loginLayout->addWidget(m_loginButton);

    m_mainLayout->addWidget(m_loginGroup);

    // ===== PROGRESS BAR =====
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    m_mainLayout->addWidget(m_progressBar);

    // ===== SECCIÓN DE ESTADO =====
    m_statusGroup = new QGroupBox("Estado y Respuestas", this);
    QVBoxLayout* statusLayout = new QVBoxLayout(m_statusGroup);

    m_statusLabel = new QLabel("Desconectado", this);
    m_statusLabel->setStyleSheet("QLabel { font-weight: bold; color: red; }");
    statusLayout->addWidget(m_statusLabel);

    m_tokenLabel = new QLabel("Token: Ninguno", this);
    m_tokenLabel->setStyleSheet("QLabel { font-size: 10px; color: gray; }");
    m_tokenLabel->setWordWrap(true);
    statusLayout->addWidget(m_tokenLabel);

    m_responseDisplay = new QTextEdit(this);
    m_responseDisplay->setReadOnly(true);
    m_responseDisplay->setMaximumHeight(200);
    m_responseDisplay->setStyleSheet("QTextEdit { background-color: #f5f5f5; font-family: monospace; }");
    statusLayout->addWidget(m_responseDisplay);

    // Botones de acción
    QHBoxLayout* actionLayout = new QHBoxLayout();
    m_viewUsersButton = new QPushButton("Ver Usuarios", this);
    m_viewUsersButton->setStyleSheet("QPushButton { background-color: #FF9800; color: white; padding: 6px; }");
    actionLayout->addWidget(m_viewUsersButton);

    m_clearLogButton = new QPushButton("Limpiar Log", this);
    actionLayout->addWidget(m_clearLogButton);

    statusLayout->addLayout(actionLayout);
    m_mainLayout->addWidget(m_statusGroup);

    // ===== CONECTAR SIGNALS =====
    connect(m_registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(m_viewUsersButton, &QPushButton::clicked, this, &MainWindow::onViewUsersClicked);
    connect(m_clearLogButton, &QPushButton::clicked, this, &MainWindow::onClearLogClicked);

    // Enter para enviar
    connect(m_registerPassword, &QLineEdit::returnPressed, this, &MainWindow::onRegisterClicked);
    connect(m_loginPassword, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);
}

void MainWindow::onRegisterClicked()
{
    QString username = m_registerUsername->text().trimmed();
    QString password = m_registerPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Por favor ingresa usuario y password");
        return;
    }

    logMessage("Registrando usuario: " + username, "blue");
    showProgress(true);

    // Crear JSON payload
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // Crear request
    QNetworkRequest request(QUrl(m_baseUrl + "/register"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Enviar POST request
    QNetworkReply* reply = m_networkManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onRegisterFinished(reply);
        reply->deleteLater();
    });
}

void MainWindow::onLoginClicked()
{
    QString username = m_loginUsername->text().trimmed();
    QString password = m_loginPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Por favor ingresa usuario y password");
        return;
    }

    logMessage("Iniciando sesión: " + username, "blue");
    showProgress(true);

    // Crear JSON payload
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // Crear request
    QNetworkRequest request(QUrl(m_baseUrl + "/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Enviar POST request
    QNetworkReply* reply = m_networkManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onLoginFinished(reply);
        reply->deleteLater();
    });
}

void MainWindow::onViewUsersClicked()
{
    logMessage("Obteniendo lista de usuarios...", "blue");
    showProgress(true);

    QNetworkRequest request(QUrl(m_baseUrl + "/users"));

    QNetworkReply* reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onViewUsersFinished(reply);
        reply->deleteLater();
    });
}

void MainWindow::onClearLogClicked()
{
    m_responseDisplay->clear();
    logMessage("Log limpiado", "gray");
}

void MainWindow::onRegisterFinished(QNetworkReply* reply)
{
    showProgress(false);

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        if (obj["success"].toBool()) {
            QString username = obj["user"].toObject()["username"].toString();
            m_currentToken = obj["token"].toString();

            logMessage("Usuario registrado: " + username, "green");
            logMessage("Token recibido", "green");

            m_statusLabel->setText("Registrado como: " + username);
            m_statusLabel->setStyleSheet("QLabel { font-weight: bold; color: green; }");

            // Limpiar campos de registro
            m_registerUsername->clear();
            m_registerPassword->clear();

            // Autocompletar campos de login
            m_loginUsername->setText(username);

            updateTokenDisplay();

        } else {
            QString error = obj["error"].toString();
            logMessage("Error: " + error, "red");
            QMessageBox::critical(this, "Error de Registro", error);
        }

    } else {
        logMessage("Error de conexión: " + reply->errorString(), "red");
        QMessageBox::critical(this, "Error de Conexión", reply->errorString());
    }
}

void MainWindow::onLoginFinished(QNetworkReply* reply)
{
    showProgress(false);

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        if (obj["success"].toBool()) {
            QString username = obj["user"].toObject()["username"].toString();
            m_currentToken = obj["token"].toString();

            logMessage("Login exitoso: " + username, "green");
            logMessage("Token actualizado", "green");

            m_statusLabel->setText("Conectado como: " + username);
            m_statusLabel->setStyleSheet("QLabel { font-weight: bold; color: green; }");

            updateTokenDisplay();

        } else {
            QString error = obj["error"].toString();
            logMessage("Login fallido: " + error, "red");
            QMessageBox::critical(this, "Error de Login", error);
        }

    } else {
        logMessage("Error de conexión: " + reply->errorString(), "red");
        QMessageBox::critical(this, "Error de Conexión", reply->errorString());
    }
}

void MainWindow::onViewUsersFinished(QNetworkReply* reply)
{
    showProgress(false);

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        if (obj["success"].toBool()) {
            QJsonArray users = obj["users"].toArray();  // ← Ahora funciona con el include

            logMessage("Usuarios registrados (" + QString::number(users.size()) + "):", "blue");

            for (const auto& userValue : users) {
                QJsonObject user = userValue.toObject();
                QString username = user["username"].toString();
                int id = user["id"].toInt();

                logMessage(QString("  • ID: %1 - Usuario: %2").arg(id).arg(username), "black");
            }

        } else {
            logMessage("Error obteniendo usuarios", "red");
        }

    } else {
        logMessage("Error de conexión: " + reply->errorString(), "red");
    }
}

void MainWindow::logMessage(const QString& message, const QString& color)
{
    QString timestamp = QTime::currentTime().toString("hh:mm:ss");
    QString formattedMessage = QString("[%1] %2").arg(timestamp, message);

    m_responseDisplay->append(QString("<font color='%1'>%2</font>").arg(color, formattedMessage));
    m_responseDisplay->ensureCursorVisible();
}

void MainWindow::showProgress(bool show)
{
    if (show) {
        m_progressBar->setVisible(true);
        m_progressBar->setRange(0, 0); // Indeterminate progress
        m_registerButton->setEnabled(false);
        m_loginButton->setEnabled(false);
        m_viewUsersButton->setEnabled(false);
    } else {
        m_progressBar->setVisible(false);
        m_registerButton->setEnabled(true);
        m_loginButton->setEnabled(true);
        m_viewUsersButton->setEnabled(true);
    }
}

void MainWindow::updateTokenDisplay()
{
    if (m_currentToken.isEmpty()) {
        m_tokenLabel->setText("Token: Ninguno");
        m_tokenLabel->setStyleSheet("QLabel { font-size: 10px; color: gray; }");
    } else {
        // Mostrar solo los primeros y últimos caracteres del token
        QString shortToken = m_currentToken.left(20) + "..." + m_currentToken.right(20);
        m_tokenLabel->setText("Token: " + shortToken);
        m_tokenLabel->setStyleSheet("QLabel { font-size: 10px; color: green; }");
    }
}

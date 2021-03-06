/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include "requestmapper.h"
#include "filelogger.h"
#include "staticfilecontroller.h"
#include "controller/dumpcontroller.h"
#include "controller/templatecontroller.h"
#include "controller/formcontroller.h"
#include "controller/fileuploadcontroller.h"
#include "controller/sessioncontroller.h"

#include "mariadbconnection.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QList>
/** Redirects log messages to a file */
extern FileLogger* logger;

/** Controller for static files */
extern StaticFileController* staticFileController;

//


RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");
    this->db = MariaDBConnection("QMYSQL","kluchens.pl","PWSZTAR","popwsz","projekt",3306);





}


RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");

}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());
/*
    // For the following pathes, each request gets its own new instance of the related controller.

    if (path.endsWith("/dump"))
    {
        DumpController().service(request, response);
    }
    else if (path.endsWith("/template"))
    {
        TemplateController().service(request, response);
    }
    else if (path.endsWith("/file"))
    {
        FileUploadController().service(request, response);
    }
    if (path.endsWith("/form"))
    {
        FormController().service(request, response);
    }
*/

    if (path.endsWith("/login"))
    {
        QString login,password;
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        if(request.getMethod() == "POST")
        {
            login = request.getParameter("login");
            password = request.getParameter("password");
        }
        if(login != NULL && password != NULL)
        {
            int userid = db.Login(login,password);
            if(userid > 0)
            {
                HttpCookie cookie;
                cookie.setName("User_ID");
                cookie.setValue(QByteArray::number(userid));
                cookie.setMaxAge(600);
                response.setCookie(HttpCookie(cookie.toByteArray()));
                response.write("<html><body><h1>Password Storage Application</h1>""<ul><li><a href='/register'>Rejestracja</a></li><li><a href='/login'>Logowanie</a></li>");
                response.write("<li><a href='/passwords?login=");
                response.write(login.toLocal8Bit());
                response.write("&password=");
                response.write(password.toLocal8Bit());
                response.write("'>Moje Hasła</a></li></ul>");
                response.write("<p>Login = ");
                response.write(request.getParameter("login")+"</p>");
                response.write("<p>Hasło = ");
                response.write(request.getParameter("password")+"</p>");
                response.write("<p>Pomyślnie zalogowano</p>");
            }
            else
            {
                response.write("<html><body><h1>Password Storage Application</h1><ul><li><a href='/register'>Rejestracja</a></li><li><a href='/login'>Logowanie</a></li><li><a href='/passwords'>Moje Hasła</a></li></ul>");
                response.write("<p>Login = ");
                response.write(request.getParameter("login")+"</p>");
                response.write("<p>Hasło = ");
                response.write(request.getParameter("password")+"</p>");
                response.write("<p>Błąd logowania</p>");
            }
        } else {
                response.write("<html><body><h1>Password Storage Application</h1><ul><li><a href='/register'>Rejestracja</a></li><li><a href='/login'>Logowanie</a></li><li><a href='/passwords'>Moje Hasła</a></li></ul>");
                response.write("<p>Login = ");
                response.write(request.getParameter("login")+"</p>");
                response.write("<p>Hasło = ");
                response.write(request.getParameter("password")+"</p>");
                response.write("<form method=\"post\">");
                response.write("  <input type=\"hidden\" name=\"action\" value=\"show\">");
                response.write("  Login: <input type=\"text\" name=\"login\"><br>");
                response.write("  Hasło: <input type=\"password\" name=\"password\"><br>");
                response.write("  <input type=\"submit\">");
                response.write("</form>");
        }
        response.write("</body></html>",true);
    }
    else if (path.endsWith("/register"))
    {
        QString login,password;
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        login = request.getParameter("login");
        password = request.getParameter("password");
        if(login != NULL && password != NULL)
        {
            if(request.getMethod() == "POST")
            {
                bool effect = db.RegisterUser( login, password);
                if(effect)
                {
                    response.write("<html><body>"
                                   "<h1>Password Storage Application</h1>"
                                   "<ul>"
                                   "<li><a href='/register'>Rejestracja</a></li>"
                                   "<li><a href='/login'>Logowanie</a></li>"
                                   "<li><a href='/passwords'>Moje Hasła</a></li>"
                                   "</ul>");
                    response.write("Pomyślnie zarejestrowano");
                } else {
                    response.write("<html><body>"
                                   "<h1>Password Storage Application</h1>"
                                   "<ul>"
                                   "<li><a href='/register'>Rejestracja</a></li>"
                                   "<li><a href='/login'>Logowanie</a></li>"
                                   "<li><a href='/passwords'>Moje Hasła</a></li>"
                                   "</ul>");
                    response.write("Błąd rejestracji!");
                }
            }
        } else {
            response.write("<html><body>"
                           "<h1>Password Storage Application</h1>"
                           "<ul>"
                           "<li><a href='/register'>Rejestracja</a></li>"
                           "<li><a href='/login'>Logowanie</a></li>"
                           "<li><a href='/passwords'>Moje Hasła</a></li>"
                           "</ul>");
            response.write("<form method=\"post\">");
            response.write("  <input type=\"hidden\" name=\"action\" value=\"show\">");
            response.write("  Login: <input type=\"text\" name=\"login\"><br>");
            response.write("  Hasło: <input type=\"password\" name=\"password\"><br>");
            response.write("  <input type=\"submit\">");
            response.write("</form>");
        }
        response.write("</body></html>",true);
        //check
    }
    else if (path.endsWith("/passwords"))
    {
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        response.write("<html><body>"
                       "<h1>Password Storage Application</h1>"
                       "<ul>"
                       "<li><a href='/register'>Rejestracja</a></li>"
                       "<li><a href='/login'>Logowanie</a></li>"
                       "<li><a href='/passwords'>Moje Hasła</a></li>"
                       "</ul>");
        QString login = request.getParameter("login");
        QString password = request.getParameter("password");
        if( login == NULL || password == NULL)
        {
                response.setStatus(401,QByteArray::fromStdString("Unauthorized"));
                response.write("Nie jesteś zalogwany");
        }
        else
        {
            response.write("<h3>Hasła</h3>");
            QList<QString>* list = db.listPasswords( login, password);
            response.write("<table>");
            response.write("<tr><th>Domena</th><th>Nazwa użytkownika</th><th>Hasło</th></tr>");
            for(int i = 0;i<list->size();)
            {
                response.write("<tr>");
                i++;
                //QString::fromStdString(str)
                response.write(QString::fromStdString("<td>"+list->at(i++).toStdString()+"</td>").toLocal8Bit());
                response.write(QString::fromStdString("<td>"+list->at(i++).toStdString()+"</td>").toLocal8Bit());
                response.write(QString::fromStdString("<td>"+list->at(i++).toStdString()+"</td>").toLocal8Bit());
                response.write("</tr>");
            }
            response.write("</table>");
        }
        response.write("</body></html>",true);
        //check
    }
    else if (path.endsWith("/app_getpasswords"))
    {
        response.setHeader("Content-Type", "application/json; charset=utf-8");
        QJsonDocument json = QJsonDocument::fromJson(request.getBody());
        QJsonObject temp = json.object();
        QString login = temp.value("login").toString();
        QString password = temp.value("password").toString();
        if(login == NULL || password == NULL)
        {
            response.setStatus(401,QByteArray::fromStdString("Unauthorized"));
            response.write(QByteArray::fromStdString("Unauthorized"),1);
        }
        else
        {
            QList<QString>* list = db.listPasswords( login, password);
            QJsonArray plot_array;
            QJsonObject item_data;
            for(int i = 0;i<list->size();)
            {
                item_data.insert("Pass_ID", QJsonValue(list->at(i++)));
                item_data.insert("Destination", QJsonValue(list->at(i++)));
                item_data.insert("Destination_User", QJsonValue(list->at(i++)));
                item_data.insert("Hashed_Password", QJsonValue(list->at(i++)));
                plot_array.push_back(QJsonValue(item_data));
            }
            QJsonDocument doc = QJsonDocument(plot_array);
            QByteArray toWrite = doc.toJson();
            response.write(toWrite,1);
            response.setStatus(200,QByteArray::fromStdString("OK"));
        }
    }
    else if (path.endsWith("/app_login"))
    {
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        QJsonDocument json = QJsonDocument::fromJson(request.getBody());
        QJsonObject temp = json.object();
        QString login = temp.value("login").toString();
        QString password = temp.value("password").toString();
        if(login != NULL && password != NULL)
        {
            int userid = db.Login(login,password);
            if(userid > 0)
            {
                HttpCookie cookie;
                cookie.setName("User_ID");
                cookie.setValue(QByteArray::number(userid));
                cookie.setMaxAge(600);
                response.setStatus(200,QByteArray::fromStdString("OK"));
                response.setCookie(HttpCookie(cookie.toByteArray()));
            }
            else
            {
                response.setStatus(400,QByteArray::fromStdString("Bad Request"));
            }
        }
        response.write("",true);
    }
    else if (path.endsWith("/app_rem_pass"))
    {
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        if(request.getMethod() == "POST")
        {
            QJsonDocument json = QJsonDocument::fromJson(request.getBody());
            QJsonObject temp = json.object();
            QString login = temp.value("login").toString();
            QString password = temp.value("password").toString();
            int pass_ID = temp.value("Pass_ID").toInt();
            int user_Id = db.Login(login,password);
            if(user_Id <= 0)
            {
                response.setStatus(401,QByteArray::fromStdString("Unauthorized"));
                response.write(QByteArray::fromStdString("Unauthorized"),1);
            }
            else
            {
                bool effect = db.RemovePassword(user_Id,pass_ID);
                if(effect)
                {
                    response.setStatus(200,QByteArray::fromStdString("OK"));
                    response.write("",true);
                } else {
                    response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                    response.write("",true);
                }
            }
        }
    }
    else if (path.endsWith("/app_mod_pass"))
    {
        QJsonDocument json = QJsonDocument::fromJson(request.getBody());
        QJsonObject temp = json.object();
        QString login = temp.value("login").toString();
        QString password = temp.value("password").toString();
        QString hashed_pass = temp.value("Hashed_Password").toString();
        QString Destination = temp.value("Destination").toString();
        QString Destination_User = temp.value("Destination_User").toString();
        QString Pass_ID = temp.value("Pass_ID").toString();
        if(db.ModifyPassword(login,password,hashed_pass,Destination,Destination_User,Pass_ID.toInt()))
        {
            response.setStatus(200,QByteArray::fromStdString("OK"));
            response.write("",true);
        } else {
            response.setStatus(400,QByteArray::fromStdString("Bad Request"));
            response.write("",true);
        }
    }
    else if (path.endsWith("/app_register"))
    {
       response.setHeader("Content-Type", "text/html; charset=utf-8");
        QJsonDocument json = QJsonDocument::fromJson(request.getBody());
        //QJsonArray jsonArray = json.array();
        QJsonObject temp = json.object();
        QString login = temp.value("login").toString();
        QString password = temp.value("password").toString();
        if(login != NULL && password != NULL)
        {
            if(request.getMethod() == "POST")
            {
                if(!db.CheckLogin(login))
                {
                    bool effect = db.RegisterUser( login, password);
                    if(effect)
                    {
                        response.setStatus(200,QByteArray::fromStdString("OK"));
                        response.write("",true);
                    } else {
                        response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                        response.write("",true);
                    }
                } else {
                    response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                    response.write("",true);
                }
            }
        } else {
            response.setStatus(400,QByteArray::fromStdString("Bad Request"));
            response.write("",true);
        }
    }
    else if (path.endsWith("/app_add_pass"))
    {
        QJsonDocument json = QJsonDocument::fromJson(request.getBody());
        //QJsonArray jsonArray = json.array();
        QJsonObject temp = json.object();
        QString login = temp.value("login").toString();
        QString password = temp.value("password").toString();
        QString Hashed_Password = temp.value("Hashed_Password").toString();
        QString Destination = temp.value("Destination").toString();
        QString Destination_User = temp.value("Destination_User").toString();
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        if(login != NULL && password != NULL)
        {
            if(request.getMethod() == "POST")
            {
                if(Hashed_Password.size() > 2 && Destination.size() > 2 && Destination_User.size() > 2)
                {
                    int user_id = db.Login(login,password);
                    if(user_id >0)
                    {
                        bool effect = db.AddPassword(user_id,Hashed_Password,Destination,Destination_User);
                        if(effect)
                        {
                            response.setStatus(200,QByteArray::fromStdString("OK"));
                            response.write("",true);
                        } else {
                            response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                            response.write("",true);
                        }
                    } else {
                        response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                        response.write("",true);

                    }
                } else {
                    response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                    response.write("",true);
                }
            }
        }
    }

    // All other pathes are mapped to the static file controller.
    // In this case, a single instance is used for multiple requests.
    else
    {
        staticFileController->service(request, response);
    }

    qDebug("RequestMapper: finished request");

    // Clear the log buffer
    if (logger)
    {
       logger->clear();
    }
}

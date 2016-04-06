/***************************************************************************
 *   Copyright (C) 2005 by Iulian M                                        *
 *   eti@erata.net                                                         *
 ***************************************************************************/
#ifndef ETKSYNCHTTP_H
#define ETKSYNCHTTP_H

#include <Q3Http>
#include <QEventLoop>
#include <QBuffer>
#include <iostream>

/**
 * Provide a synchronous api over Q3Http
 * Uses a QEventLoop to block until the request is completed
 * @author Iulian M <eti@erata.net>
*/
class SyncHTTP: public Q3Http
{
    Q_OBJECT
    public:
        /// structors
        SyncHTTP( QObject * parent = 0 )
        :Q3Http(parent),requestID(-1),status(false){}

        SyncHTTP( const QString & hostName, quint16 port = 80, QObject * parent = 0 )
        :Q3Http(hostName,port,parent),requestID(-1),status(false){}

        virtual ~SyncHTTP(){}

        /// send GET request and wait until finished
        bool syncGet ( const QString & path, QIODevice * to = 0 )
        {
            ///connect the requestFinished signal to our finished slot
            connect(this,SIGNAL(requestFinished(int,bool)),SLOT(finished(int,bool)));
            /// start the request and store the requestID
            requestID = get(path, to );
            /// block until the request is finished
            loop.exec();
            /// return the request status
            (*to).open(QIODevice::ReadWrite);
            return status;
        }

        bool syncGetQString(const QString & path, QString & resultStr) {

            QBuffer output;
            bool status = syncGet(path,&output);
            if (status == true) {
                output.open(QIODevice::ReadWrite);
                QByteArray bytes = output.readAll();
                resultStr = QString(bytes);
                std::cout << "resultStr: " << resultStr.toStdString().c_str() << std::endl;
                return true;
            } else {
                std::cout << "FAILLLLLLL" << std::endl;
                return status;
            }
        }

        /// send POST request and wait until finished
        bool syncPost ( const QString & path, QIODevice * data, QIODevice * to = 0 )
        {
            ///connect the requestFinished signal to our finished slot
            connect(this,SIGNAL(requestFinished(int,bool)),SLOT(finished(int,bool)));
            /// start the request and store the requestID
            requestID = post(path, data , to );
            /// block until the request is finished
            loop.exec();
            /// return the request status
            return status;
        }

        bool syncPost ( const QString & path, const QByteArray& data, QIODevice * to = 0 )
        {
            /// create io device from QByteArray
            QBuffer buffer;
            buffer.setData(data);
            return syncPost(path,&buffer,to);
        }

    protected Q_SLOTS:
        virtual void finished(int idx, bool err)
        {
            /// check to see if it's the request we made
            if(idx!=requestID)
                return;
            /// set status of the request
            status = !err;
            /// end the loop
            loop.exit();
        }

    private:
        /// id of current request
        int requestID;
        /// error status of current request
        bool status;
        /// event loop used to block until request finished
        QEventLoop loop;
};

#endif

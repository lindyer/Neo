#include "HttpClient.h"

#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QHash>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QHttpMultiPart>

class HttpClientPrivate {
public:
	HttpClientPrivate(const QString &url);

	QString   url;    // ����� URL
	QUrlQuery params; // ����Ĳ���ʹ�� Form ��ʽ
	QString   json;   // ����Ĳ���ʹ�� Json ��ʽ
	QHash<QString, QString> headers; // ����ͷ
	QNetworkAccessManager  *manager;

	bool useJson; // Ϊ true ʱ����ʹ�� Json ��ʽ���ݲ���������ʹ�� Form ��ʽ���ݲ���
	bool debug;   // Ϊ true ʱ�������� URL �Ͳ���

	// HTTP ���������
	enum HttpMethod {
		GET, POST, PUT, DELETE, UPLOAD /* UPLOAD ���� HTTP Method��ֻ��Ϊ���ϴ�ʱ���⴦�������� */
	};

	/**
	 * @brief ��ȡ Manager�����ʹ�ô���� manager �򷵻ش� manager�������´���һ�� manager
	 * @param d        HttpClientPrivate �Ķ���
	 * @param internal ʹ�ô���� manager �� interval ������Ϊ false�������µ� manager ������ interval Ϊ true
	 * @return ���� QNetworkAccessManager ����
	 */
	static QNetworkAccessManager* getManager(HttpClientPrivate *d, bool *internal);

	/**
	 * @brief ʹ���û��趨�� URL������ͷ�ȴ��� Request
	 * @param d      HttpClientPrivate �Ķ���
	 * @param method ���������
	 * @return ���ؿ�����ִ������� QNetworkRequest
	 */
	static QNetworkRequest createRequest(HttpClientPrivate *d, HttpMethod method);

	/**
	 * @brief ִ������ĸ�������
	 * @param d              HttpClient �ĸ�������
	 * @param method         ���������
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param encoding       ������Ӧ�ı���
	 */
	static void executeQuery(HttpClientPrivate *d, HttpMethod method,
							 std::function<void(const QString &)> successHandler,
							 std::function<void(const QString &)> errorHandler,
							 const char *encoding);

	/**
	 * @brief �ϴ��ļ���������
	 * @param d     HttpClientPrivate �Ķ���
	 * @param paths Ҫ�ϴ����ļ���·��(path �� data ����ͬʱʹ��)
	 * @param data  Ҫ�ϴ����ļ�������
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param encoding       ������Ӧ�ı���
	 */
	static void upload(HttpClientPrivate *d,
					   const QStringList &paths, const QByteArray &data,
					   std::function<void(const QString &)> successHandler,
					   std::function<void(const QString &)> errorHandler,
					   const char *encoding);

	/**
	 * @brief ��ȡ��������Ӧ������
	 * @param reply    ����� QNetworkReply ����
	 * @param encoding ������Ӧ�ı��룬Ĭ��ʹ�� UTF-8
	 * @return ����������Ӧ���ַ���
	 */
	static QString readReply(QNetworkReply *reply, const char *encoding = "UTF-8");

	/**
	 * @brief ��������Ĵ�����
	 * @param debug          ���Ϊ true �����������Ϣ��Ϊ false �����
	 * @param successMessage ����ɹ�����Ϣ
	 * @param errorMessage   ����ʧ�ܵ���Ϣ
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param reply          QNetworkReply ���󣬲���Ϊ NULL
	 * @param manager        ����� manager����Ϊ NULL ʱ�ڴ˺����� delete
	 */
	static void handleFinish(bool debug,
							 const QString &successMessage,
							 const QString &errorMessage,
							 std::function<void(const QString &)> successHandler,
							 std::function<void(const QString &)> errorHandler,
							 QNetworkReply *reply, QNetworkAccessManager *manager);
};

HttpClientPrivate::HttpClientPrivate(const QString &url) : url(url), manager(NULL), useJson(false), debug(false) {
}

// ע��: ��Ҫ�ڻص�������ʹ�� d����Ϊ�ص�����������ʱ HttpClient ����ܿ����Ѿ����ͷŵ��ˡ�
HttpClient::HttpClient(const QString &url) : d(new HttpClientPrivate(url)) {
}

HttpClient::~HttpClient() {
	delete d;
}

HttpClient &HttpClient::manager(QNetworkAccessManager *manager) {
	d->manager = manager;
	return *this;
}

// ���� debug Ϊ true ��ʹ�� debug ģʽ������ִ��ʱ�������� URL �Ͳ�����
HttpClient &HttpClient::debug(bool debug) {
	d->debug = debug;

	return *this;
}

// ��� Form ��ʽ����
HttpClient &HttpClient::param(const QString &name, const QString &value) {
	d->params.addQueryItem(name, value);

	return *this;
}

// ��� Json ��ʽ����
HttpClient &HttpClient::json(const QString &json) {
	d->useJson = true;
	d->json = json;

	return *this;
}

// ��ӷ���ͷ
HttpClient &HttpClient::header(const QString &header, const QString &value) {
	d->headers[header] = value;

	return *this;
}

// ִ�� GET ����
void HttpClient::get(std::function<void(const QString &)> successHandler,
					 std::function<void(const QString &)> errorHandler,
					 const char *encoding) {
	HttpClientPrivate::executeQuery(d, HttpClientPrivate::GET, successHandler, errorHandler, encoding);
}

// ִ�� POST ����
void HttpClient::post(std::function<void(const QString &)> successHandler,
					  std::function<void(const QString &)> errorHandler,
					  const char *encoding) {
	HttpClientPrivate::executeQuery(d, HttpClientPrivate::POST, successHandler, errorHandler, encoding);
}

// ִ�� PUT ����
void HttpClient::put(std::function<void(const QString &)> successHandler,
					 std::function<void(const QString &)> errorHandler,
					 const char *encoding) {
	HttpClientPrivate::executeQuery(d, HttpClientPrivate::PUT, successHandler, errorHandler, encoding);
}

// ִ�� DELETE ����
void HttpClient::remove(std::function<void(const QString &)> successHandler,
						std::function<void(const QString &)> errorHandler,
						const char *encoding) {
	HttpClientPrivate::executeQuery(d, HttpClientPrivate::DELETE, successHandler, errorHandler, encoding);
}

void HttpClient::download(const QString &destinationPath,
						  std::function<void(const QString &)> successHandler,
						  std::function<void(const QString &)> errorHandler) {
	bool  debug = d->debug;
	QFile *file = new QFile(destinationPath);

	if (file->open(QIODevice::WriteOnly)) {
		download([=](const QByteArray &data) {
			file->write(data);
		}, [=](const QString &) {
			// ����������ͷ��ļ�����
			file->flush();
			file->close();
			file->deleteLater();

			if (debug) {
				qDebug().noquote() << QString("������ɣ����浽: %1").arg(destinationPath);
			}

			if (NULL != successHandler) {
				successHandler(QString("������ɣ����浽: %1").arg(destinationPath));
			}
		}, errorHandler);
	} else {
		// ���ļ�����
		if (debug) {
			qDebug().noquote() << QString("���ļ�����: %1").arg(destinationPath);
		}

		if (NULL != errorHandler) {
			errorHandler(QString("���ļ�����: %1").arg(destinationPath));
		}
	}
}

// ʹ�� GET �������أ��������ݿɶ�ȡʱ�ص� readyRead(), ����������Ӧ���� readyRead() ������ݱ��浽�ļ�
void HttpClient::download(std::function<void(const QByteArray &)> readyRead,
						  std::function<void(const QString &)> successHandler,
						  std::function<void(const QString &)> errorHandler) {
	bool debug = d->debug;
	bool internal;
	QNetworkAccessManager *manager = HttpClientPrivate::getManager(d, &internal);
	QNetworkRequest        request = HttpClientPrivate::createRequest(d, HttpClientPrivate::GET);
	QNetworkReply           *reply = manager->get(request);

	// �����ݿɶ�ȡʱ�ص� readyRead()
	QObject::connect(reply, &QNetworkReply::readyRead, [=] {
		readyRead(reply->readAll());
	});

	// �������
	QObject::connect(reply, &QNetworkReply::finished, [=] {
		QString successMessage = "�������"; // �������ʱһ���Զ�ȡ������Ӧ����
		QString errorMessage = reply->errorString();
		HttpClientPrivate::handleFinish(debug, successMessage, errorMessage, successHandler, errorHandler,
										reply, internal ? manager : NULL);
	});
}

// �ϴ��ļ�
void HttpClient::upload(const QString &path,
						std::function<void(const QString &)> successHandler,
						std::function<void(const QString &)> errorHandler,
						const char *encoding) {
	QStringList paths = (QStringList() << path);
	HttpClientPrivate::upload(d, paths, QByteArray(), successHandler, errorHandler, encoding);
}

// �ϴ�����
void HttpClient::upload(const QByteArray &data,
						std::function<void(const QString &)> successHandler,
						std::function<void(const QString &)> errorHandler,
						const char *encoding) {
	HttpClientPrivate::upload(d, QStringList(), data, successHandler, errorHandler, encoding);
}

void HttpClient::upload(const QStringList &paths,
						std::function<void(const QString &)> successHandler,
						std::function<void(const QString &)> errorHandler,
						const char *encoding) {
	HttpClientPrivate::upload(d, paths, QByteArray(), successHandler, errorHandler, encoding);
}

// �ϴ��ļ��������ݵ�ʵ��
void HttpClientPrivate::upload(HttpClientPrivate *d,
							   const QStringList &paths, const QByteArray &data,
							   std::function<void(const QString &)> successHandler,
							   std::function<void(const QString &)> errorHandler,
							   const char *encoding) {
	bool debug = d->debug;
	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	// ���� Form ���Ĳ��� Text Part
	QList<QPair<QString, QString> > paramItems = d->params.queryItems();
	for (int i = 0; i < paramItems.size(); ++i) {
		QHttpPart textPart;
		QString name = paramItems.at(i).first;
		QString value = paramItems.at(i).second;
		textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"").arg(name));
		textPart.setBody(value.toUtf8());
		multiPart->append(textPart);
	}

	if (paths.size() > 0) {
		// �ϴ��ļ�
		QString inputName = paths.size() == 1 ? "file" : "files"; // һ���ļ�ʱΪ file������ļ�ʱΪ files

		for (const QString &path : paths) {
			if (!path.isEmpty()) {
				// path ��Ϊ��ʱ���ϴ��ļ�
				QFile *file = new QFile(path);
				file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

				// ����ļ���ʧ�ܣ����ͷ���Դ����
				if (!file->open(QIODevice::ReadOnly)) {
					QString errorMessage = QString("���ļ�ʧ��[%2]: %1").arg(path).arg(file->errorString());

					if (debug) {
						qDebug().noquote() << errorMessage;
					}

					if (NULL != errorHandler) {
						errorHandler(errorMessage);
					}

					multiPart->deleteLater();
					return;
				}

				// �ļ��ϴ��Ĳ�����Ϊ file��ֵΪ�ļ���
				// �������� Java ������ form-data
				// �������� PHP  ������ multipart/form-data
				QString   disposition = QString("form-data; name=\"%1\"; filename=\"%2\"").arg(inputName).arg(file->fileName());
				QHttpPart filePart;
				filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(disposition));
				filePart.setBodyDevice(file);
				multiPart->append(filePart);
			}
		}
	} else {
		// �ϴ�����
		QString   disposition = QString("form-data; name=\"file\"; filename=\"no-name\"");
		QHttpPart dataPart;
		dataPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(disposition));
		dataPart.setBody(data);
		multiPart->append(dataPart);
	}

	bool internal;
	QNetworkAccessManager *manager = HttpClientPrivate::getManager(d, &internal);
	QNetworkRequest        request = HttpClientPrivate::createRequest(d, HttpClientPrivate::UPLOAD);
	QNetworkReply           *reply = manager->post(request, multiPart);

	QObject::connect(reply, &QNetworkReply::finished, [=] {
		multiPart->deleteLater(); // �ͷ���Դ: multiPart + file

		QString successMessage = HttpClientPrivate::readReply(reply, encoding); // �������ʱһ���Զ�ȡ������Ӧ����
		QString errorMessage = reply->errorString();
		HttpClientPrivate::handleFinish(debug, successMessage, errorMessage, successHandler, errorHandler,
										reply, internal ? manager : NULL);
	});
}

// ִ������ĸ�������
void HttpClientPrivate::executeQuery(HttpClientPrivate *d, HttpMethod method,
									 std::function<void(const QString &)> successHandler,
									 std::function<void(const QString &)> errorHandler,
									 const char *encoding) {
	// �����ʹ���ⲿ�� manager �򴴽�һ���µģ��ڷ�����ɺ���Զ�ɾ����
	bool debug = d->debug;
	bool internal;
	QNetworkAccessManager *manager = HttpClientPrivate::getManager(d, &internal);
	QNetworkRequest        request = HttpClientPrivate::createRequest(d, method);
	QNetworkReply           *reply = NULL;

	switch (method) {
	case HttpClientPrivate::GET:
		reply = manager->get(request);
		break;
	case HttpClientPrivate::POST:
		reply = manager->post(request, d->useJson ? d->json.toUtf8() : d->params.toString(QUrl::FullyEncoded).toUtf8());
		break;
	case HttpClientPrivate::PUT:
		reply = manager->put(request, d->useJson ? d->json.toUtf8() : d->params.toString(QUrl::FullyEncoded).toUtf8());
		break;
	case HttpClientPrivate::DELETE:
		reply = manager->deleteResource(request);
		break;
	default:
		break;
	}

	QObject::connect(reply, &QNetworkReply::finished, [=] {
		QString successMessage = HttpClientPrivate::readReply(reply, encoding); // �������ʱһ���Զ�ȡ������Ӧ����
		QString errorMessage = reply->errorString();
		HttpClientPrivate::handleFinish(debug, successMessage, errorMessage, successHandler, errorHandler,
										reply, internal ? manager : NULL);
	});
}

QNetworkAccessManager* HttpClientPrivate::getManager(HttpClientPrivate *d, bool *internal) {
	*internal = d->manager == NULL;
	return *internal ? new QNetworkAccessManager() : d->manager;
}

QNetworkRequest HttpClientPrivate::createRequest(HttpClientPrivate *d, HttpMethod method) {
	bool get = method == HttpMethod::GET;
	bool upload = method == HttpClientPrivate::UPLOAD;
	bool postForm = !get && !upload && !d->useJson;
	bool postJson = !get && !upload &&  d->useJson;

	// ����� GET ���󣬲��Ҳ�����Ϊ�գ����������Ĳ������ŵ� URL ����
	if (get && !d->params.isEmpty()) {
		d->url += "?" + d->params.toString(QUrl::FullyEncoded);
	}

	// ����ʱ�����ַ�Ͳ���
	if (d->debug) {
		qDebug().noquote() << "��ַ:" << d->url;

		if (postJson) {
			qDebug().noquote() << "����:" << d->json;
		} else if (postForm || upload) {
			QList<QPair<QString, QString> > paramItems = d->params.queryItems();
			QString buffer; // �����ε��� qDebug() ���������Ϣ��ÿ�� qDebug() ���п�������кŵ�

			// ����ֵ�Եķ�ʽ�������
			for (int i = 0; i < paramItems.size(); ++i) {
				QString name = paramItems.at(i).first;
				QString value = paramItems.at(i).second;

				if (0 == i) {
					buffer += QString("����: %1=%2\n").arg(name).arg(value);
				} else {
					buffer += QString("     %1=%2\n").arg(name).arg(value);
				}
			}

			if (!buffer.isEmpty()) {
				qDebug().noquote() << buffer;
			}
		}
	}

	// ����� POST ����useJson Ϊ true ʱ��� Json ������ͷ��useJson Ϊ false ʱ��� Form ������ͷ
	if (postForm) {
		d->headers["Content-Type"] = "application/x-www-form-urlencoded";
	} else if (postJson) {
		d->headers["Accept"] = "application/json; charset=utf-8";
		d->headers["Content-Type"] = "application/json";
	}

	// �������ͷ��ӵ� request ��
	QNetworkRequest request(QUrl(d->url));
	QHashIterator<QString, QString> iter(d->headers);
	while (iter.hasNext()) {
		iter.next();
		request.setRawHeader(iter.key().toUtf8(), iter.value().toUtf8());
	}

	return request;
}

QString HttpClientPrivate::readReply(QNetworkReply *reply, const char *encoding) {
	QTextStream in(reply);
	QString result;
	in.setCodec(encoding);

	while (!in.atEnd()) {
		result += in.readLine();
	}

	return result;
}

void HttpClientPrivate::handleFinish(bool debug,
									 const QString &successMessage,
									 const QString &errorMessage,
									 std::function<void(const QString &)> successHandler,
									 std::function<void(const QString &)> errorHandler,
									 QNetworkReply *reply, QNetworkAccessManager *manager) {
	if (reply->error() == QNetworkReply::NoError) {
		// ����ɹ�
		if (debug) {
			qDebug().noquote() << QString("[�ɹ�]�������: %1").arg(successMessage);
		}

		if (NULL != successHandler) {
			successHandler(successMessage);
		}
	} else {
		// ����ʧ��
		if (debug) {
			qDebug().noquote() << QString("[ʧ��]�������: %1").arg(errorMessage);
		}

		if (NULL != errorHandler) {
			errorHandler(errorMessage);
		}
	}

	// �ͷ���Դ
	reply->deleteLater();

	if (NULL != manager) {
		manager->deleteLater();
	}
}
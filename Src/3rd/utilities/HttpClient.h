#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <functional>

class QString;
class QStringList;
class QByteArray;
class QNetworkRequest;
class QNetworkReply;
class QNetworkAccessManager;
class HttpClientPrivate;

/**
 * �� QNetworkAccessManager ���м򵥷�װ�� HTTP ���ʿͻ��ˣ��� GET��POST��PUT��DELETE���ϴ������صȲ�����
 * ִ������ɵ��� get(), post(), put(), remove(), download(), upload()��
 * ��ִ������ǰ�ɵ��� header() ��������ͷ������ʹ�� Form ���ķ�ʽ��������� param()���������ʹ�� request body
 * ��������� json() ���ò���(��ȻҲ���Բ��� JSON ��ʽ��ʹ�� request body ����������� RESTful ʱ����Ҷ����� JSON ��ʽ��������Ϊ json)��
 * Ĭ�� HttpClient �ᴴ��һ�� QNetworkAccessManager���������ʹ��Ĭ�ϵģ����� manager() ���뼴�ɡ�
 * Ĭ�ϲ�����������ַ�����ȵ�����Ϣ�������Ҫ��������� debug(true) ���ɡ�
 */
class HttpClient {
public:
	HttpClient(const QString &url);
	~HttpClient();

	/**
	 * @brief ÿ����һ�� QNetworkAccessManager ���󶼻ᴴ��һ���̣߳���Ƶ���ķ�������ʱ��Ϊ�˽�ʡ�߳���Դ��
	 *     ���Դ��� QNetworkAccessManager �����������(�����ᱻ HttpClient ɾ�����û���Ҫ�Լ��ֶ�ɾ��)��
	 *     ���û��ʹ�� useManager() ����һ�� QNetworkAccessManager���� HttpClient ���Զ��Ĵ���һ�������������������ɺ�ɾ������
	 * @param  manager QNetworkAccessManager ����
	 * @return ���� HttpClient �����ã�����������ʽ����
	 */
	HttpClient& manager(QNetworkAccessManager *manager);

	/**
	 * @brief  ���� debug Ϊ true ��ʹ�� debug ģʽ������ִ��ʱ�������� URL �Ͳ�����
	 * @param  debug �Ƿ����õ���ģʽ
	 * @return ���� HttpClient �����ã�����������ʽ����
	 */
	HttpClient& debug(bool debug);

	/**
	 * @brief �������Ĳ���
	 * @param name  ����������
	 * @param value ������ֵ
	 * @return ���� HttpClient �����ã�����������ʽ����
	 */
	HttpClient& param(const QString &name, const QString &value);

	/**
	 * @brief �������Ĳ�����ʹ�� Json ��ʽ������ "{\"name\": \"Alice\"}"
	 * @param json Json ��ʽ�Ĳ����ַ���
	 * @return
	 */
	HttpClient& json(const QString &json);

	/**
	 * @brief �������ͷ
	 * @param header ����ͷ������
	 * @param value  ����ͷ��ֵ
	 * @return ���� HttpClient �����ã�����������ʽ����
	 */
	HttpClient& header(const QString &header, const QString &value);

	/**
	 * @brief ִ�� GET ����
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param encoding       ������Ӧ�ı���
	 */
	void get(std::function<void(const QString &)> successHandler,
			 std::function<void(const QString &)> errorHandler = NULL,
			 const char *encoding = "UTF-8");

	/**
	 * @brief ִ�� POST ����
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param encoding       ������Ӧ�ı���
	 */
	void post(std::function<void(const QString &)> successHandler,
			  std::function<void(const QString &)> errorHandler = NULL,
			  const char *encoding = "UTF-8");

	/**
	 * @brief ִ�� PUT ����
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param encoding       ������Ӧ�ı���
	 */
	void put(std::function<void(const QString &)> successHandler,
			 std::function<void(const QString &)> errorHandler = NULL,
			 const char *encoding = "UTF-8");

	/**
	 * @brief ִ�� DELETE ����
	 *        ���� delete �� C++ ���������������ͬ��� remove
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param encoding       ������Ӧ�ı���
	 */
	void remove(std::function<void(const QString &)> successHandler,
				std::function<void(const QString &)> errorHandler = NULL,
				const char *encoding = "UTF-8");

	/**
	 * @brief ʹ�� GET �������أ����ص��ļ����浽 savePath
	 * @param savePath       ���ص��ļ�����·��
	 * @param successHandler ��������ɺ�Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ���������ļ� destinationPath ����Ҳ����ô˺���
	 */
	void download(const QString &savePath,
				  std::function<void(const QString &)> successHandler = NULL,
				  std::function<void(const QString &)> errorHandler = NULL);

	/**
	 * @brief ʹ�� GET �������أ��������ݿɶ�ȡʱ�ص� readyRead(), ����������Ӧ���� readyRead() ������ݱ��浽�ļ�
	 * @param readyRead      �����ݿɶ�ȡʱ�Ļص� lambda ����
	 * @param successHandler ��������ɺ�Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 */
	void download(std::function<void(const QByteArray &)> readyRead,
				  std::function<void(const QString &)> successHandler = NULL,
				  std::function<void(const QString &)> errorHandler = NULL);

	/**
	 * @brief �ϴ������ļ�
	 * @param path Ҫ�ϴ����ļ���·��
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param encoding       ������Ӧ�ı���
	 */
	void upload(const QString &path, std::function<void(const QString &)> successHandler = NULL,
				std::function<void(const QString &)> errorHandler = NULL,
				const char *encoding = "UTF-8");

	/**
	 * @brief �ϴ����������ļ�
	 * @param path Ҫ�ϴ����ļ���·��
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param encoding       ������Ӧ�ı���
	 */
	void upload(const QByteArray &data, std::function<void(const QString &)> successHandler = NULL,
				std::function<void(const QString &)> errorHandler = NULL,
				const char *encoding = "UTF-8");

	/**
	 * @brief �ϴ�����ļ�
	 * @param paths Ҫ�ϴ����ļ���·��
	 * @param successHandler ����ɹ��Ļص� lambda ����
	 * @param errorHandler   ����ʧ�ܵĻص� lambda ����
	 * @param encoding       ������Ӧ�ı���
	 */
	void upload(const QStringList &paths, std::function<void(const QString &)> successHandler = NULL,
				std::function<void(const QString &)> errorHandler = NULL,
				const char *encoding = "UTF-8");
private:
	HttpClientPrivate *d;
};

#endif // HTTPCLIENT_H
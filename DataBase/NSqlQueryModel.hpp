#ifndef __NSQLQUERYMODEL_HPP__
#define __NSQLQUERYMODEL_HPP__

#include <QSqlQueryModel>
#include <QSqlQuery>

class NSqlQueryModel : public QSqlQueryModel
{
        Q_OBJECT

    public:

        /*!
         * \brief NSqlQueryModel : default constructor
         * \param parent : pointer to an Object parent if exists
         */
        explicit NSqlQueryModel(QObject* parent = 0);

        /*!
         * \brief refresh : exec the last query to update the model
         */
        void refresh(void);

    signals:

    public slots:

};

#endif /* __NSQLQUERYMODEL_HPP__ */

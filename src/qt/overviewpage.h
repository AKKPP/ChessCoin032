#ifndef OVERVIEWPAGE_H
#define OVERVIEWPAGE_H

#include <QWidget>
#include <QImage>

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

namespace Ui {
    class OverviewPage;
}
class WalletModel;
class TxViewDelegate;
class TransactionFilterProxy;

/** Overview ("home") page widget */
class OverviewPage : public QWidget
{
    Q_OBJECT

public:
    explicit OverviewPage(QWidget *parent = 0);
    ~OverviewPage();

    void setModel(WalletModel *model);
    void showOutOfSyncWarning(bool fShow);
    bool getOutOfSyncWarning(void);

public slots:
    void setBalance(qint64 balance, qint64 stake, qint64 unconfirmedBalance, qint64 immatureBalance);

signals:
    void transactionClicked(const QModelIndex &index);

protected:
    void paintEvent(QPaintEvent *event)	override;

private:
    Ui::OverviewPage *ui;
    WalletModel *model;
    qint64 currentBalance;
    qint64 currentStake;
    qint64 currentUnconfirmedBalance;
    qint64 currentImmatureBalance;

    TxViewDelegate *txdelegate;
    TransactionFilterProxy *filter;

    bool is_out_of_sync;

    QImage imgLogo;

private slots:
    void updateDisplayUnit();
    void handleTransactionClicked(const QModelIndex &index);
};

#endif // OVERVIEWPAGE_H

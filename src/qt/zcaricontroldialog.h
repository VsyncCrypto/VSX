// Copyright (c) 2017-2020 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZCARICONTROLDIALOG_H
#define ZCARICONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "zcari/zerocoin.h"

class CZerocoinMint;
class WalletModel;

namespace Ui {
class ZCariControlDialog;
}

class CZCariControlWidgetItem : public QTreeWidgetItem
{
public:
    explicit CZCariControlWidgetItem(QTreeWidget *parent, int type = Type) : QTreeWidgetItem(parent, type) {}
    explicit CZCariControlWidgetItem(int type = Type) : QTreeWidgetItem(type) {}
    explicit CZCariControlWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent, type) {}

    bool operator<(const QTreeWidgetItem &other) const;
};

class ZCariControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZCariControlDialog(QWidget *parent);
    ~ZCariControlDialog();

    void setModel(WalletModel* model);

    static std::set<std::string> setSelectedMints;
    static std::set<CMintMeta> setMints;
    static std::vector<CMintMeta> GetSelectedMints();

private:
    Ui::ZCariControlDialog *ui;
    WalletModel* model;

    void updateList();
    void updateLabels();

    enum {
        COLUMN_CHECKBOX,
        COLUMN_DENOMINATION,
        COLUMN_PUBCOIN,
        COLUMN_VERSION,
        COLUMN_CONFIRMATIONS,
        COLUMN_ISSPENDABLE
    };
    friend class CZCariControlWidgetItem;

private Q_SLOTS:
    void updateSelection(QTreeWidgetItem* item, int column);
    void ButtonAllClicked();
};

#endif // ZCARICONTROLDIALOG_H

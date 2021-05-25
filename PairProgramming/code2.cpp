//编辑事件
void MainWindow::onActionEdit()
{
    QDate date = ui->calendarWidget->selectedDate();
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    if (items.empty()) {
        return;
    }
    QString edit_s = items.first()->text();

    QVector<schedule>* temp = ScheduleOperate->getschedule(date);
    for (int i = 0; i < temp->size(); ++i)
    {
        if ((*temp)[i].getname() == edit_s && !(*temp)[i].get_is_delete())
        {
            //记录下更改前的事件的内容
            QString detail = (*temp)[i].getdetail();
            QDateTime start = (*temp)[i].getstart();
            QDateTime end = (*temp)[i].getend();
            //弹出对话框，用户在对话框中对原本的事件信息编辑
            dialog_edit = new Dialog(edit_s, start, end, detail, this);
            dialog_edit->setModal(true);
            dialog_edit->show();
            dialog_edit->exec();

            //更改时间信息后点击了对话框的“确定”按钮，确定更改
            if (dialog_edit->result() == dialog_edit->Accepted)
            {
                //记录下更改后的事件信息
                QString t = dialog_edit->getTitle();
                QString d = dialog_edit->getdetail();
                QDateTime s = dialog_edit->getstart();
                QDateTime e = dialog_edit->getend();
                //必须保证结束时间大于开始时间
                if (e < s)
                {
                    QMessageBox::information(this, "警告", "结束时间不能早于开始时间");
                    return;
                }

                //确保事件标题不能为空
                if (t.isEmpty())
                {
                    QMessageBox::information(this, "警告", "事件名不能为空");
                    return;
                }

                //找到该日期下的原事件，将原事件删除
                QVector<schedule>* temp = ScheduleOperate->getschedule(date);
                for (int i = 0; i < temp->size(); ++i)
                {
                    if ((*temp)[i].getname() == edit_s && !(*temp)[i].get_is_delete())
                    {
                        QDate D1 = (*temp)[i].getstart().date();
                        QDate D2 = (*temp)[i].getend().date();
                        while (D1 <= D2)
                        {
                            QVector<schedule>* temp1 = ScheduleOperate->getschedule(D1);
                            for (int i = 0; i < temp1->size(); i++)
                            {
                                if ((*temp1)[i].getname() == edit_s && !(*temp1)[i].get_is_delete())
                                {
                                    (*temp1)[i].Delete();
                                }
                            }
                            D1 = D1.addDays(1);
                        }
                        break;
                    }
                }
                //重新加入编辑后的事件
                schedule sche(t, d, s, e);
                QDate D3 = s.date();
                QDate D4 = e.date();
                while (D3 <= D4)
                {
                    ScheduleOperate->add_schedule(D3, sche);
                    D3 = D3.addDays(1);
                }
            }

            //在listWidght上显示当天的事件
            ui->listWidget->clear();//首先清空原有的内容
            QDate date = ui->calendarWidget->selectedDate();//获取日历空间所选中的日期
            QVector<schedule>* temp = ScheduleOperate->getschedule(date);//获取该日期下的所有事件的集合
            if (temp == nullptr)
            {
                ui->listWidget->clear();
            }
            else
            {
                ui->listWidget->clear();
                if (temp != nullptr)
                {
                    for (int i = 0; i < temp->size(); ++i)
                    {
                        if (!(*temp)[i].get_is_delete())
                        {
                            ui->listWidget->addItem((*temp)[i].getname());
                        }
                    }
                }
            }
            delete dialog_edit;
            return;
        }
    }
}
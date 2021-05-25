//�༭�¼�
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
            //��¼�¸���ǰ���¼�������
            QString detail = (*temp)[i].getdetail();
            QDateTime start = (*temp)[i].getstart();
            QDateTime end = (*temp)[i].getend();
            //�����Ի����û��ڶԻ����ж�ԭ�����¼���Ϣ�༭
            dialog_edit = new Dialog(edit_s, start, end, detail, this);
            dialog_edit->setModal(true);
            dialog_edit->show();
            dialog_edit->exec();

            //����ʱ����Ϣ�����˶Ի���ġ�ȷ������ť��ȷ������
            if (dialog_edit->result() == dialog_edit->Accepted)
            {
                //��¼�¸��ĺ���¼���Ϣ
                QString t = dialog_edit->getTitle();
                QString d = dialog_edit->getdetail();
                QDateTime s = dialog_edit->getstart();
                QDateTime e = dialog_edit->getend();
                //���뱣֤����ʱ����ڿ�ʼʱ��
                if (e < s)
                {
                    QMessageBox::information(this, "����", "����ʱ�䲻�����ڿ�ʼʱ��");
                    return;
                }

                //ȷ���¼����ⲻ��Ϊ��
                if (t.isEmpty())
                {
                    QMessageBox::information(this, "����", "�¼�������Ϊ��");
                    return;
                }

                //�ҵ��������µ�ԭ�¼�����ԭ�¼�ɾ��
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
                //���¼���༭����¼�
                schedule sche(t, d, s, e);
                QDate D3 = s.date();
                QDate D4 = e.date();
                while (D3 <= D4)
                {
                    ScheduleOperate->add_schedule(D3, sche);
                    D3 = D3.addDays(1);
                }
            }

            //��listWidght����ʾ������¼�
            ui->listWidget->clear();//�������ԭ�е�����
            QDate date = ui->calendarWidget->selectedDate();//��ȡ�����ռ���ѡ�е�����
            QVector<schedule>* temp = ScheduleOperate->getschedule(date);//��ȡ�������µ������¼��ļ���
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
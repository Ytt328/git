//����¼���ť
void MainWindow::on_pushbutton_add_clicked()
{
    //�����Ի����û���������Ӧ����
    dialog = new Dialog(this);
    dialog->setModal(true);
    dialog->show();
    dialog->exec();

    //����˶Ի���ġ�ȷ������ť����������¼�
    if (dialog->result() == dialog->Accepted)
    {
        //��ȡ�û�������¼��������Ϣ
        QString t = dialog->getTitle();//�¼���
        QDateTime s = dialog->getstart();//�¼���ʼʱ��
        QDateTime e = dialog->getend();//�¼�����ʱ��
        QString d = dialog->getdetail();//�¼���ϸ��Ϣ

        //ȷ������ʱ�䲻��С�ڿ�ʼʱ��
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

        //���û����������ʵ����һ��schedule����
        schedule sche(t, d, s, e);

        //��ȡ��ʼʱ��ͽ���ʱ�������
        QDate D1 = s.date();
        QDate D2 = e.date();

        //��ʼ���ںͽ���������ͬһ�죬ֻ���ڸ����ڵ��¼���������Ӹ��¼�
        if (D2 == D1)
        {
            ScheduleOperate->add_schedule(D1, sche);
        }
        else if (D2 > D1)
        {
            //��ʼ���ںͽ������ڲ���ͬһ�죬������������֮����������ڶ�Ҫ��Ӹ��¼�
            while (D1 <= D2)
            {
                ScheduleOperate->add_schedule(D1, sche);
                D1 = D1.addDays(1);
            }
        }
    }

    //��listWidght����ʾ�����ؼ���ǰ��ѡ��������µ������¼�
    QDate date = ui->calendarWidget->selectedDate();//��ȡ�����ؼ���ǰ��ѡ������
    QVector<schedule>* temp = ScheduleOperate->getschedule(date);//��ȡ�������µ������¼��ļ���
    if (temp == nullptr)
    {
        //��ǰ������û���¼������listWidght���ݼ���
        ui->listWidget->clear();
    }
    else
    {
        //��ǰ���������¼�
        //�����listWidght����
        ui->listWidget->clear();
        //���������������¼�������ʾ�¼����⵽listWidght��
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
    delete dialog;
}
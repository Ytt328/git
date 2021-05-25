//添加事件按钮
void MainWindow::on_pushbutton_add_clicked()
{
    //弹出对话框，用户可输入相应内容
    dialog = new Dialog(this);
    dialog->setModal(true);
    dialog->show();
    dialog->exec();

    //点击了对话框的“确定”按钮，即可添加事件
    if (dialog->result() == dialog->Accepted)
    {
        //获取用户输入的事件的相关信息
        QString t = dialog->getTitle();//事件名
        QDateTime s = dialog->getstart();//事件开始时间
        QDateTime e = dialog->getend();//事件结束时间
        QString d = dialog->getdetail();//事件详细信息

        //确保结束时间不能小于开始时间
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

        //用用户输入的数据实例化一个schedule对象
        schedule sche(t, d, s, e);

        //获取开始时间和结束时间的日期
        QDate D1 = s.date();
        QDate D2 = e.date();

        //开始日期和结束日期在同一天，只用在该日期的事件集合中添加该事件
        if (D2 == D1)
        {
            ScheduleOperate->add_schedule(D1, sche);
        }
        else if (D2 > D1)
        {
            //开始日期和结束日期不在同一天，则这两个日期之间的所有日期都要添加该事件
            while (D1 <= D2)
            {
                ScheduleOperate->add_schedule(D1, sche);
                D1 = D1.addDays(1);
            }
        }
    }

    //在listWidght中显示日历控件当前所选择的日期下的所有事件
    QDate date = ui->calendarWidget->selectedDate();//获取日历控件当前所选的日期
    QVector<schedule>* temp = ScheduleOperate->getschedule(date);//获取该日期下的所有事件的集合
    if (temp == nullptr)
    {
        //当前日期下没有事件，清空listWidght内容即可
        ui->listWidget->clear();
    }
    else
    {
        //当前日期下有事件
        //先清空listWidght内容
        ui->listWidget->clear();
        //遍历日期下所有事件，并显示事件标题到listWidght上
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
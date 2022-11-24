#include<iostream>
#include<iomanip>
#include<ctime>
#include<vector>
#include<limits>
#include<windows.h>


using namespace std;

//xu ly loi input
int input_num(string prompt)
{
	int ch;
	cout << prompt;
	cin >> ch;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "\nKhong hop le. Vui long nhap lai. \n" << prompt;
		cin >> ch;
	}
	return ch;
}

//nut tin nhan SLLs
class msg
{
	public:
		bool star;			//true neu tin nhan duoc ghim
		bool sent;			//true neu tin nhan da duoc gui den dung nguoi 
		bool read;			//true neu tin nhan da duoc nguoi dung da dang nhap doc
		string dt; 			//date & time tin nhan  duoc nhan/gui
		string to;			//username  nguoi dung can gui tin nhan
		string from;		//username  nguoi dung tin nhan ban nhan duoc
		string text;		// tin nhan
		msg *link;

		msg()
		{
			star = false;
			sent = false;
			read = true;
			to = "";
			from = "";
			text = "";
			link = NULL;
		}
};

//node cho  user DLL( doule linklist)
class user
{
	public:
		bool logged_in;
		string username;
		string password;
		msg *headS; 			//sent msg SLL head
		msg *headR; 			//received msg SLL head
		vector<msg*> trash;		//vector of deleted msg
		user *next;
		user *prev;
		friend class messager;
		user()
		{
			logged_in = false;
			username = "";
			password = "";
			headS = NULL;
			headR = NULL;
			next = NULL;
			prev = NULL;
		}

		void display_msgs(string title, msg *head);				//hien thi danh sach cua tin nhan gui/ nhan 
		void msg_options(string title, msg **head);				//thao tac nguoi dung thuc hien voi danh sach hien thi
		void read_msg(msg *head);								//doc mot tin nhan nhat dinh
		void del_msg(msg **head);								//xoa tin nhan nhat dinh
		void starUnstar_msg(msg *m);							//danh dau tin nhan ghim hoac bo ghim 
		void vec_read_msg(vector<msg*> results);				//doc tin nhan tu ket qua tim kiem 
		void vec_del_msg(vector<msg*> results, msg **head); 	//xoa tin nhan tu ket qua tim kiem
		void vec_starUnstar(vector<msg*> results); 				//ghim hoac bo ghim tin nhan tu ket qua tim kiem
		void search_msg(string title, msg **head);				//tim kiem tin nhan duoc nhan /gui tu mot user
		void starred_msg(string title, msg **head);				//hien thi tin nhan ghim
		void trash_options();									//thao tac voi tin nhan da xoa
		void del_permanently();									//xoa tin nhan tu thung rac (permanently)
		void read_trashMsg();									//doc tin nhan tu thung rac

};


void user::display_msgs(string title, msg *head)
{
	string R[] = { "chua doc", "da doc" };
	string S[] = { "chua ghim", "da ghim" };
	cout <<"\n******************************* " <<title<< " *******************************";

	if (head == NULL)
		cout << "\nKhong co tin nhan nao de hien thi!\n";

	else
	{
		int i = 1;
		cout << "\n-------------------------------------------------------------------------------------------------";
		cout << "\n" << setw(5) << "No." << setw(10) << "Tu" << setw(10)
				<< "Den" << setw(15) << "Tinnhan" << setw(20) << "Thoigian"
				<< setw(15) << "Trangthai" << setw(15) << "Ghim";
		cout << "\n-------------------------------------------------------------------------------------------------";

		msg *m = head;
		while (m != NULL)
		{
			cout << "\n" << setw(5) << i << setw(10) << m->from << setw(10)
					<< m->to << setw(15) << m->text.substr(0, 8) << "..."
					<< setw(15) << m->dt.substr(4, 6) << setw(15) << R[m->read]
					<< setw(20) << S[m->star];
			cout << "\n-------------------------------------------------------------------------------------------------";
			m = m->link;
			i++;
		}
	}
}


void user::msg_options(string title, msg **head)
{
	int ch;
	do
	{
		display_msgs(title, *head);
		if (*head == NULL)
			return;
		cout << "\n********* " << title << " - LUACHON **********";
		cout << "\n0. Thoat";
		cout << "\n1. Doc 1 tin nhan";
		cout << "\n2. Xoa tin nhan";
		cout << "\n3. Ghim /Bo ghim tin nhan";
		ch = input_num("\nNhap lua chon: ");
		cout << "\n---------------------------------------------";

		switch (ch)
		{
			case 0:
				break;

			case 1:
				read_msg(*head);
				break;

			case 2:
				del_msg(head);
				break;

			case 3:
				starUnstar_msg(*head);
				break;
		}
	} while (ch != 0);
}


void user::read_msg(msg *head)
{
	int no;
	no = input_num("\nNhap tin nhan no. de doc: ");
	if (no < 1)
	{
		cout << "\nKhong ton tai tin nhan no.";
		return;
	}
	msg *ptr = head;
	for (int i = 1; i < no; i++)
	{
		ptr = ptr->link;
		if (ptr == NULL)
		{
			cout << "\nKhong ton tai tin nhan no.";
			return;
		}
	}
	cout << "\n..................................................................";
	cout << "\n************** TINNHAN " << no << " **************";
	cout << "\nFrom : " << ptr->from;
	cout << "\nTo : " << ptr->to;
	cout << "\nThoigian : " << ptr->dt;
	cout << "\nTinnhan : \n" << ptr->text;
	cout << "\n...................................................................\n";
	ptr->read = true;
}


void user::del_msg(msg **head)
{
	if (*head == NULL)
	{
		cout << "Khong tim thay tin nhan.\n";
		return;
	}

	int no = input_num("\nNhap tin nhan no. de xoa: ");

	if (no < 1)
	{
		cout << "\nKhong ton tai tin nhan no.";
		return;
	}

	msg *ptr = *head;
	msg *prev = *head;
	if (no == 1)
	{
		*head = (*head)->link;
		cout << "Tin nhan duoc xoa thanh cong!!\n";
		trash.push_back(ptr);
		return;
	}

	for (int i = 1; i < no; i++)
	{
		prev = ptr;
		ptr = ptr->link;
		if (ptr == NULL)
		{
			cout << "Khong ton tai tin nhan no.\n";
			return;
		}
	}
	prev->link = ptr->link;
	trash.push_back(ptr);
	cout << "Tin nhan duoc xoa thanh cong!!\n";
}


void user::starUnstar_msg(msg *head)
{
	int no = input_num("\nNhap tin nhan no. de ghim/bo ghim: ");

	if (no < 1)
	{
		cout << "\nKhong ton tai tin nhan no.";
		return;
	}
	msg *ptr = head;
	for (int i = 1; i < no; i++)
	{
		ptr = ptr->link;
		if (ptr == NULL)
		{
			cout << "\nKhong ton tai tin nhan no.";
			return;
		}
	}
	if (ptr->star == false)
	{
		ptr->star = true;
		cout << "Tin nhan  no. " << no << " da duoc ghim.\n";
	}
	else
	{
		ptr->star = false;
		cout << "Tin nhan no. " << no << " da bo ghim.\n";
	}
}

void user::vec_read_msg(vector<msg*> results)
{
	unsigned int no = unsigned(input_num("\nNhap tin nhan no. de doc: "));

	if (no < 1 || no > results.size())
	{
		cout << "\nKhong ton tai tin nhan no.";
		return;
	}

	msg *ptr = results.at(no - 1);
	cout << "\n..................................................................";
	cout << "\n************** TINNHAN " << no << " **************";
	cout << "\nFrom : " << ptr->from;
	cout << "\nTo : " << ptr->to;
	cout << "\nThoigian : " << ptr->dt;
	cout << "\nTinnhan : \n" << ptr->text;
	cout << "\n...................................................................\n";
	ptr->read = true;
}


void user::vec_del_msg(vector<msg*> results, msg **head)
{
	unsigned int no = unsigned(input_num("\nNhap tin nhan  no. de xoa: "));

	if (no < 1 || no > results.size())
	{
		cout << "\nKhong ton tai tin nhan no.";
		return;
	}

	msg *ptr = *head;
	msg *prev = *head;

	if (results.at(no - 1) == *head)
	{
		*head = (*head)->link;
		cout << "\nTin nhan duoc xoa thanh cong !!";
		trash.push_back(ptr);
		results.erase(results.begin() + no - 1);
		return;
	}
	for (ptr = *head; ptr != results.at(no - 1);)
	{
		prev = ptr;
		ptr = ptr->link;
		if (ptr == NULL)
		{
			cout << "Khong ton tai tin nhan no.\n";
			return;
		}
	}
	prev->link = ptr->link;
	ptr = results.at(no - 1);
	trash.push_back(ptr);
	results.erase(results.begin() + no - 1);
	cout << "Tin nhan duoc xoa thanh cong!!\n";
}


void user::vec_starUnstar(vector<msg*> results)
{
	unsigned int no = unsigned(input_num("\nNhap tin nhan no. de ghim/bo ghim: "));

	if (no < 1 || no > results.size())
	{
		cout << "\nKhong ton tai tin nhan no.";
		return;
	}

	msg *ptr = results.at(no - 1);
	if (ptr->star)
	{
		ptr->star = false;
		cout << "Tin nhan no. " << no << " da duoc bo ghim.\n";
	}
	else
	{
		ptr->star = true;
		cout << "Tin nhan no. " << no << " da duoc ghim.\n";
	}
}


void user::search_msg(string title, msg **head)
{
	string un;
	cout << "\nNhap vao ten dang nhap: ";
	cin >> un;

	bool found = false;
	msg *m = *head;
	if (*head == NULL)
	{
		cout << "\nChua co tin nhan nao duoc hien thi!";
		return;
	}

	string cmp;

	string R[] =  { "chua doc", "da doc" };
	string S[] =  { "chua ghim", "daghim" };

	int ch, i;
	do
	{
		i = 0;
		found = false;
		vector<msg*> results;

		for (m = *head; m != NULL; m = m->link)
		{
			if (title == "GUI DEN ")
				cmp = m->to;
			else 
				cmp = m->from;

			if (cmp == un)
			{
				if (!found)
				{
					cout<<"\n**************************** TINNHAN " <<title<< un << " ****************************";
					cout<< "\n---------------------------------------------------------------------------------------------------------------";
					cout<< "\n" << setw(5) << "No." << setw(10) << "Tu"
							<< setw(10) << "Den" << setw(15) << "Tinnhan"
							<< setw(20) << "Thoigian" << setw(15) << "Trangthai"
							<< setw(15) << "Ghim";
					cout<< "\n--------------------------------------------------------------------------------------------------------------";

				}
				i++;
				found = true;
				results.push_back(m);

				cout << "\n" << setw(5) << i << setw(10) << m->from << setw(10)
						<< m->to << setw(15) << m->text.substr(0, 8) << "..."
						<< setw(15) << m->dt.substr(4, 6) << setw(15)
						<< R[m->read] << setw(20) << S[m->star];
				cout << "\n-------------------------------------------------------------------------------------------------";
			}
		}

		if (m == NULL && !found)
		{
			cout << "\nKhong tim thay tin nhan!\n";
			return;
		}

		cout << "\n********* TINNHAN - LUACHON **********";
		cout << "\n0. Thoat";
		cout << "\n1. Doc mot tin nhan";
		cout << "\n2. Xoa mot tin nhan";
		cout << "\n3. Ghim/Bo ghim mot tin nhan";
		ch = input_num("\nNhap lua chon: ");
		cout << "\n---------------------------------------------";

		switch (ch)
		{
			case 0:
				break;

			case 1:
				vec_read_msg(results);
				break;

			case 2:
				vec_del_msg(results, head);
				break;

			case 3:
				vec_starUnstar(results);
				break;
		}
	} while (ch != 0);
}


void user::starred_msg(string title, msg **head)
{

	string R[] = { "chua doc", "da doc" };
	string S[] = { "chua ghim", "da ghim" };

	msg *m = *head;
	if (head == NULL)
	{
		cout << "\nKhong co tin nhan nao duoc hien thi!";
		return;
	}

	int ch, i;
	bool found;
	vector<msg*> results;
	do
	{
		i = 0;
		found = false;
		vector<msg*> results;

		for (m = *head; m != NULL; m = m->link)
		{

			if (m->star == true)
			{

				if (!found)
				{
					cout << "\n**************************** TINNHAN DA GHIM TRONG "<< title << " ****************************";
					cout << "\n-------------------------------------------------------------------------------------------------";
					cout << "\n" << setw(5) << "No." << setw(10) << "Tu"
							<< setw(10) << "Den" << setw(15) << "Tinnhan"
							<< setw(20) << "Thoigian" << setw(15) << "Trangthai"
							<< setw(15) << "Ghim";
					cout << "\n-------------------------------------------------------------------------------------------------";
				}
				i++;
				found = true;
				results.push_back(m);

				cout << "\n" << setw(5) << i << setw(10) << m->from << setw(10)
						<< m->to << setw(15) << m->text.substr(0, 8) << "..."
						<< setw(15) << m->dt.substr(4, 6) << setw(15)
						<< R[m->read] << setw(20) << S[m->star];
				cout << "\n-------------------------------------------------------------------------------------------------";
			}
		}

		if (m == NULL && !found)
		{
			cout << "\nKhong tim thay tin nhan!\n";
			return;
		}

		cout << "\n********* TINNHAN - LUACHON **********";
		cout << "\n0. Thoat";
		cout << "\n1. Doc mot tin nhan";
		cout << "\n2. Xoa mot tin nhan";
		cout << "\n3. Ghim/Bo ghim mot tin nhan";
		ch = input_num("\nNhap lua chon: ");
		cout << "\n---------------------------------------------";

		switch (ch)
		{
			case 0:
				break;

			case 1:
				vec_read_msg(results);
				break;

			case 2:
				vec_del_msg(results, head);
				break;

			case 3:
				vec_starUnstar(results);
				break;
		}
	} while (ch != 0);

}


void user::trash_options()
{
	int ch;
	do
	{
		string R[] = { "chua doc", "da doc" };
		string S[] = { "chua ghim", "da ghim" };
		if (this->trash.size() == 0)
		{
			cout << "Thung rac rong\n";
			return;
		}
		cout<< "\n******************************* THUNG RAC *******************************";
		cout<< "\n-------------------------------------------------------------------------------------------------";
		cout << "\n" << setw(5) << "No." << setw(10) << "Tu" << setw(10) << "Den"
				<< setw(15) << "Tinnhan" << setw(20) << "Thoigian" << setw(15)
				<< "Trangthai" << setw(15) << "Ghim";
		cout<< "\n-------------------------------------------------------------------------------------------------";

		for (unsigned int i = 0; i < this->trash.size(); i++)
		{
			msg *m = this->trash[i];
			cout << "\n" << setw(5) << i+1 << setw(10) << m->from << setw(10)
								<< m->to << setw(15) << m->text.substr(0, 8) << "..."
								<< setw(15) << m->dt.substr(4, 6) << setw(15) << R[m->read]<< setw(15) << S[m->star];
			cout << "\n-------------------------------------------------------------------------------------------------";
		}

		cout << "\n********* THUNGRAC - LUACHON **********";
		cout << "\n0. Thoat";
		cout << "\n1. Xoa mot tin nhan vinh vien";
		cout << "\n2. Xem mot tin nhan";
		ch = input_num("\nNhap lua chon: ");
		cout << "\n---------------------------------------------";

		switch (ch)
		{
			case 0:
				break;

			case 1:
				del_permanently();
				break;

			case 2:
				read_trashMsg();
				break;
		}
	} while (ch != 0);
}


void user::del_permanently()
{
	unsigned int no = unsigned(input_num("\nNhap tin nhan no. de xoa: "));
	if (no > trash.size() || no < 1)
	{
		cout << "Khong ton tai tin nhan no.\n";
		return;
	}

	msg *m = trash[no - 1];
	trash.erase(trash.begin() + no - 1);
	cout << "Tin nhan da duoc xoa vinh vien\n";
	delete m;
}


void user::read_trashMsg()
{
	unsigned int no = unsigned(input_num("\nNhap tin nhan no. de doc : "));

	if (no < 1 || no > trash.size())
	{
		cout << "\nKhong ton tai tin nhan no.";
		return;
	}

	cout << "\n..................................................................";
	cout << "\n************** TINNHAN " << no << " **************";
	cout << "\nTu : " << trash[no - 1]->from;
	cout << "\nDen : " << trash[no - 1]->to;
	cout << "\nThoigian : " << trash[no - 1]->dt;
	cout << "\nTinnhan : \n" << trash[no - 1]->text;
	cout << "\n...................................................................\n";
	trash[no - 1]->read = true;
}

class messager
{
		user *start;		//start pointer of user DLL
		user *last;			//pointer to last node of user DLL
	public:
		messager()
		{
			start = NULL;
			last = NULL;
		}

		bool is_empty();			
		user* accept();				//yêu cau dau vao khi tao tai khoan
		void create(); 				//tao tai khoan và them vao  user dll(sign-up)
		void login();  				//login vao tai khoan da co 
		void remove(); 				//xoa tai khoan khoi danh sach tai khoan da luu
		void change_pw();			//doi mat khau
		void activity(user *ptr); 		//thao tac nguoi dung co the làm khi dang nhap vao tai khoan
		msg* msg_sent(); 				//nhan input de gui tin nhan, update hop thu den cua nguoi nhan và tra ve con tro de gui nhan
		void send_msg(user *ptr); 		//goi msg_sent () và cap nhat msg sll dã gui cua ngu?i dùng
};


bool messager::is_empty()
{
	if (start == NULL)
		return true;
	else
		return false;
}

//yêu cau dau vao khi tao tai khoan
user* messager::accept()
{
	bool un_exist = false;
	user *tmp = new user();
	user *ptr;
	do
	{
		ptr = start;
		cout << "\nNhap ten dang nhap de tao tai khoan : ";
		cin >> tmp->username;
		while (ptr != NULL)
		{
			if (ptr->username == tmp->username)
			{
				cout << "\nTen dang nhap da ton tai.";
				un_exist = true;
				break;
			}
			ptr = ptr->next;
		}
		if (ptr == NULL)
			un_exist = false;
	} while (un_exist);
	cout << "\nTao mat khau: ";
	cin >> tmp->password;
	return tmp;
}

//tao tai khoan và them vao  user dll(sign-up)
void messager::create()
{
	user *tmp = accept();
	if (is_empty())
	{
		start = tmp;
		last = tmp;
	}
	else
	{
		last->next = tmp;
		tmp->prev = last;
		last = tmp;
	}
	cout << "\nTao tai khoan thanh cong!";
}

//login vao tai khoan da co 
void messager::login()
{
	string un, pw;
	cout << "\nNhap ten dang nhap: ";
	cin >> un;
	for (user *ptr = start; ptr != NULL; ptr = ptr->next)
	{
		if (ptr->username == un)
		{
			cout << "\nNhap mat khau: ";
			cin >> pw;
			if (ptr->password == pw)
			{
				ptr->logged_in = true;
				cout << "\nDang nhap thanh cong";
				activity(ptr);
				return;
			}
			else
			{
				cout << "\nMat khau sai. Thu lai.";
				return;
			}
		}
	}
	cout << "\nKhong ton tai nguoi dung";
}

//xoa tai khoan khoi danh sach tai khoan da luu
void messager::remove()
{
	char ch;
	string un, pw;

	cout << "\nNhap ten dang nhap: ";
	cin >> un;

	if (is_empty())
		cout << "\nKhong co nguoi dung nao.";

	for (user *curr = start; curr != NULL; curr = curr->next)
	{
		if (curr->username == un)
		{
			cout << "\nNhap mat khau: ";
			cin >> pw;

			if (curr->password == pw)
			{
				do
				{
					cout << "Ban co chac chan muon xoa tai khoan ?(Y/N): ";
					cin >> ch;
					if (ch == 'N') return;
					else if (ch == 'Y') break;
					else cout << "\nLua chon khong hop le. Thu lai.";
				} while (ch != 'Y' || ch != 'N');

				//if start node
				if (curr == start)
					start = curr->next;

				//if curr is not last node
				if (curr->next != NULL)
					(curr->next)->prev = curr->prev;

				//if curr is not 1st node
				if (curr->prev != NULL)
					(curr->prev)->next = curr->next;

				delete curr;
				cout << "\nTai khoan cua ban da xoa vinh vien!";
				return;
			}
			else
			{
				cout << "\nSai mat khau .... vui long thu lai.";
				return;
			}
		}
	}
	cout << "\nKhong ton tai nguoi dung";
}

//doi mat khau
void messager::change_pw()
{
	string un, pw, pw1;
	cout << "\nNhap ten dang nhap: ";
	cin >> un;

	for (user *ptr = start; ptr != NULL; ptr = ptr->next)
	{
		if (ptr->username == un)
		{
			do
			{
				cout << "\nNhap mat khau truoc do: ";
				cin >> pw;
				if (ptr->password == pw)
				{
					cout << "\nNhap mat khau moi : ";
					cin >> pw1;
					ptr->password = pw1;

					cout << "\nMat khau cua ban da doi thanh cong!";
					return;
				}
				else
					cout << "\nMat khau truoc do khong chinh xac.\n";
			} while (true);
		}
	}
	cout << "\nKhong ton tai nguoi dung";
}

//thao tac nguoi dung co the làm khi dang nhap vao tai khoan
void messager::activity(user *ptr)
{
	int ch;
	do
	{
		cout << "\n************* HELLO @"<< ptr->username<< " ! *************";
		cout << "\n0. Dang xuat";
		cout << "\n1. Kiem tra tin nhan den ";
		cout << "\n2. Gui mot tin nhan";
		cout << "\n3. Xem tin nhan da gui";
		cout << "\n4. Tim kiem tin nhan da gui ";
		cout << "\n5. Tim kiem tin nhan duoc nhan";
		cout << "\n6. Xem tin nhan da xoa";
		cout << "\n7. Xem tin nhan da ghim trong Inbox ";
		cout << "\n8. Xem tin nhan da ghim trong SentBox";
		ch = input_num("\nNhap lua chon: ");
		cout << "\n------------------------------------------\n";

		switch (ch)
		{
			case 0:
				ptr->logged_in = false;
				cout << "\nDang xuat thanh cong.";
				return;

			case 1:
				ptr->msg_options("HOP THU DEN", &ptr->headR);
				break;

			case 2:
				send_msg(ptr);
				break;

			case 3:
				ptr->msg_options("GUI", &ptr->headS);
				break;

			case 4:
				ptr->search_msg("GUI DEN ", &ptr->headS);
				break;

			case 5:
				ptr->search_msg("NHAN TU ", &ptr->headR);
				break;

			case 6:
				ptr->trash_options();
				break;

			case 7:
				ptr->starred_msg("HOP THU DEN ", &ptr->headR);
				break;

			case 8:
				ptr->starred_msg("HOP THU GUI ", &ptr->headS);
				break;

			default:
				cout << "\nLua chon khong hop le";
		}

	} while (ch != 0);
}

//nhan input de gui tin nhan, update hop thu den cua nguoi nhan và tra ve con tro de guii tin nhan
msg* messager::msg_sent()
{
	msg *m = new msg();
	user *ptrT; 				//ptr To tai khoan nguoi dang gui tin nhan 
	bool un_exist = false;
	do
	{
		cout << "Nhap ten nguoi can gui tin nhan : ";
		cin >> m->to;
		getchar(); //'\n'

		//updating 
		for (ptrT = start; ptrT != NULL; ptrT = ptrT->next)
		{
			if (ptrT->username == m->to)
			{

				cout << "\nNhap tin nhan ban muon gui den @" <<m->to<< " :\n";
				getline(cin, m->text);

				m->read = false;
				time_t now = time(0); 		// ngày / gio hien tai tren he thong
				m->dt = ctime(&now);		// chuyen sang chuoi
				un_exist = true;
				cout << "\nTin nhan da gui den  @" << m->to;

				//chen tin nhan moi vào dau sll tin nhan da nhan cua ptrT
				m->link = ptrT->headR;
				ptrT->headR = m;
				return m;
			}
		}

		if (ptrT == NULL) //!un_exist
			cout << "\nTen da nhap khong ton tai.\n";

	} while (un_exist == false);
	return m;
}

//goi msg_sent () và cap nhat msg sll dã gui cua ngu?i dùng
void messager::send_msg(user *ptr)
{
	msg *ms = msg_sent(); 		//pointer  sent msg
	ms->from = ptr->username;

	//updating
	msg *m = new msg();			//tao tin nhan moi de cap nhat sll mag da gui c?a ngu?i dùng
	m->sent = true;
	m->to = ms->to;
	m->from = ms->from;
	m->dt = ms->dt;
	m->text = ms->text;

	//chen tin nha da gui vao dau sll msg da gui
	m->link = ptr->headS;
	ptr->headS = m;
}

int main()
{
	cout << "\n ------------------------------------- ";
	cout << "\n||          BAO CAO MON HOC            ||";
    cout << "\n||   CAU TRUC DU LIEU VA GIAI THUAT    ||";
    cout << "\n ------------------------------------- ";
    
 
    cout << "\n ****************  NHOM 26  **************** ";
    cout << "\n ----------------------------------------- ";
    cout << "\n ----Thanhvien----                            --MSSV--";
    cout << "\n" <<setw(5) <<"1.Tran Nguyen Quang Lam " << left <<setw(30)<<right <<"20139040";
    cout << "\n" <<setw(5) <<"2.Le Duc Phong " << left << setw(39) << right <<"20139084";
    cout << "\n" <<setw(5) <<"3.To Gia Huy " <<left<< setw(41) <<right <<"20139003";
    cout << "\n" <<setw(5) <<"4.Nguyen Luong Phu Gia " <<left <<setw(31)<< right <<"20139072";
    cout << "\n                                                                           ";
    cout << "\n                                                                           ";
    cout << "\n                                                                           ";
    cout << "\n                                                                           ";


	int ch;
	messager A;
	do
	{
		
		cout << "\n----------------------------------------";
		cout << "\n******** WELCOME TO MESSAGER **********";
		cout << "\n0. Thoat ung dung.";
		cout << "\n1. Tao tai khoan moi.";
		cout << "\n2. Dang nhap vao tai khoan cua ban.";
		cout << "\n3. Xoa mot tai khoan dang ton tai";
		cout << "\n4. Thay doi mat khau";
		ch = input_num("\nNhap lua chon: ");
		cout << "\n----------------------------------------";

		switch (ch)
		{
			case 0:
				cout << "\n********* PROGRAM ENDED **********";
				break;

			case 1:
				A.create();
				break;

			case 2:
				A.login();
				break;

			case 3:
				A.remove();
				break;

			case 4:
				A.change_pw();
				break;
			default:
				cout << "\nLua chon khong hop le";
		}

	} while (ch != 0);
	return 0;
}

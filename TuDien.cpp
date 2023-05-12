#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>

#define NGHIA_TOI_DA 5	// so nghia toi da cua mot tu
#define CR_KTK 50 // chieu rong khung tim kiem
#define HIEN_THI_TOI_DA 15 // so tu hien thi toi da o man hinh chinh

using namespace std;

/* cac cau truc du lieu chinh cua chuong trinh */
struct NODEVIDU {
	string info;
	NODEVIDU *next;
};

struct AWORD {
	string tu;
	string loai;
	string nghia[NGHIA_TOI_DA];
	NODEVIDU *first;
};

struct NODEWORD {
	AWORD info;
	NODEWORD *left;
	NODEWORD *right;
};

struct hashtable {
	NODEWORD *head;
	NODEWORD *tail;	
};

int tapTuDien(string);

/* cac bien toan cuc */ 
bool dulieuThaydoi = false; // bien ghi nhan du lieu co thay doi hay khong
string TEN_UNG_DUNG = "TU DIEN ANH-VIET \t\t\t\t | PGS.TS Hoang Van Dung  |";

/* cac ham xu ly giao dien */
// Ham di chuyen con tro den vi tri nao do trong Console
void gotoxy(short x, short y) {
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x, y};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

// Ham thay doi textcolor va backgroundcolor
void setColor(short x) { 
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, x);
}

/* cac ham xu ly danh sach lien ket don VI DU */
// chen 1 vi du vao dau ds lien ket don
void chenVdVaoDauDs(NODEVIDU *&first, string vidu) {
	// tao not moi trung gian
	NODEVIDU *p = new NODEVIDU();
	p->info = vidu;
	p->next = NULL;
	// truong hop ds chua co phan tu nao
	if (first == NULL) {
		first = p;
	} else {
		// truong hop ds da co it nhat 1 phan tu
		p->next = first;
		first = p;
	}
}

// chen 1 tu vao cuoi ds tu dien
void chenTuVaoCuoiTd(hashtable *&tudien, NODEWORD *p) {
	int i = tapTuDien(p->info.tu);
	// truong hop tap tu dien chua co tu nao
	if (tudien[i].head == NULL) {
		tudien[i].head = tudien[i].tail = p;
	} else {
		// truong hop tap tu dien da co it nhat 1 tu
		tudien[i].tail->right = p;
		p->left = tudien[i].tail;
		tudien[i].tail = p;
	}
}

// xac dinh xem 1 tu thuoc ve tap tu dien nao
int tapTuDien(string p) {
	return p[0] - 97;
}

NODEWORD* chuyendoi(string);


void docFile(hashtable *&tudien) {
	ifstream fi("words.txt"); // tim tap tin
	string dong;
	
	if (fi.is_open()) { // mo tap tin, kiem tra tap tin co ton tai hay khong...
		while (getline(fi, dong)) {
			//cout << dong << endl;
			if (dong == "") continue;
			NODEWORD *p = chuyendoi(dong);
			chenTuVaoCuoiTd(tudien, p);
		}
		fi.close(); // dong tap tin
	}
}

// chuyen du lieu tung dong thanh cau truc du lieu
NODEWORD* chuyendoi(string dong) {
	NODEWORD *p = new NODEWORD();
	int i, pos;
	i = 0;
	// tach tu
	string tu = "";
	while (dong[i] != '/') {tu += dong[i++];}
	i++;
	p->info.tu = tu;
	// tach loai tu
	string loai = "";
	while (dong[i] != '/') {loai += dong[i++];}
	i++;
	p->info.loai = loai;
	
	// tach nghia
	string nghia = ""; pos = 0;
	while (dong[i] != '/') {
		nghia += dong[i];
		i++; 
		// nghia moi
		if (dong[i] == ';') { 
			p->info.nghia[pos++] = nghia;
			nghia = "";
			i++;
		}
	}
	i++;
	
	// tach vi du
	p->info.first = NULL;
	string vidu = "";
	while (dong[i] != '/') {
		vidu += dong[i++];
		if (dong[i] == ';') {
			chenVdVaoDauDs(p->info.first, vidu);
			vidu = "";
			i++;	
		}
	}
		
	return p;
}

void ghiFile(hashtable *tudien) {
	ofstream fo("words.txt");
	if(fo.is_open()) {
		
		for (int i = 0; i < 26; i++) {
			NODEWORD *p = tudien[i].head;
			if (p == NULL) continue;
			while (p != NULL) {
				fo << p->info.tu << "/";
				fo << p->info.loai << "/";
				
				for (int i = 0; i < NGHIA_TOI_DA; i++) {
					if (p->info.nghia[i] == "") break;
					fo << p->info.nghia[i] << ";";
				}
				fo << "/";
				
				NODEVIDU *vd = p->info.first;
				while (vd != NULL) {
					fo << vd->info << ";";
					vd = vd->next;
				}
				fo << "/\n";
				p = p->right;
			}
		}
		
		fo.close();
	}
}

void xoaManHinh() {
	system("cls");
}

void veGiaoDienChiTietTu(NODEWORD *p) {
	xoaManHinh();
	int dongHienTai = 0;
	
	// in ra cac huong dan
	// o man hinh chi tiet tu
	cout << " " << char(254) << " Esc      : Ve lai man hinh chinh" << endl;
	cout << " " << char(254) << " Tab      : Sua tu nay" << endl;
	cout << " " << char(254) << " Backspace: Xoa tu nay" << endl;
	
	// in ra cac chi tiet cua tu
	cout << endl << " " << p->info.tu;
	cout << endl << " ---";
	cout << endl << " (" << p->info.loai << "): ";
	for (int i = 0; i < NGHIA_TOI_DA; i++) {
		if (p->info.nghia[i] == "") break;
		cout << p->info.nghia[i] << "; ";
	}
	cout << endl << " ---";
	
	NODEVIDU *v = p->info.first;
	while (v != NULL) {
		cout << endl << " " <<  v->info;
		v = v->next;
	}
}

// so sanh input voi mot tu cu the
bool soSanh(string input, NODEWORD *p) {
	string tu = p->info.tu;	
	// khi tu nhap ngan hon tu can so
	for (int i = 0; i < input.size(); i++) {
		if (input[i] != tu[i]) {
			return false;
		}
	}
	return true;
}

NODEWORD* timKiem(hashtable *tudien, string input) {
	int index = tapTuDien(input);
	NODEWORD *p = tudien[index].head;
	while (p != NULL) {
		if (soSanh(input, p)) {
			return p;
		}
		p = p->right;
	}	
	return NULL;
}

NODEWORD* inDanhSachTu(hashtable *tudien, string input, int pos) {
	// in ra ds cac tu
	NODEWORD *tuHienTai = NULL; // tu dang duoc highlight trong man hinh chinh
	
	// neu khung tim kiem k co gia tri, thi in het cac tu ra
	int index = -1;
	if (input != "") {
		index = tapTuDien(input);
	}
	
	int count = 0;
	for (int i = 0; i < 26; i++) {
		if (tudien[i].head == NULL) continue; // tap tu dien nay khong tu nao, bo qua
		NODEWORD *p = tudien[i].head;
		if (index != -1) {
			p = timKiem(tudien, input);
		}
		while (true) {
			while (pos > 0 && p != NULL) {
				pos--;
				p = p->right;
			}
			if (p == NULL) break;
			if (count == 0) {
				tuHienTai = p;
				setColor(207);	
			} else {
				setColor(15);	
			}
			cout << endl << " " << p->info.tu << " ";
			for (int j = 0; j < CR_KTK - p->info.tu.size(); j++) {
				cout << " ";
			}
			count++;
			p = p->right;
			if (p == NULL) {
				break;
			}
		}
		if (index != -1) break;
		if (count >= HIEN_THI_TOI_DA) break;
	}
	setColor(15);
	return tuHienTai;
}

// ve man hinh chinh
// tra ve tu dang duoc highlight
NODEWORD* veGiaoDienChinh(hashtable *tudien, string input, int pos) {
	xoaManHinh();
	int dongHienTai = 0;
	
	// in ra cac huong dan o man hinh chinh
	cout << " " << TEN_UNG_DUNG << endl;	
	cout << " " << char(254) << " Esc  : Thoat chuong trinh \t\t\t |Nguyen Hien     20161190|" << endl;
	cout << " " << char(254) << " Tab  : Them tu moi \t\t\t\t |Nguyen Tuan Anh 20119195|" << endl;
	cout << " " << char(254) << " Enter: Xem nghia cua tu \t\t\t |Bo Thien Khai   20119025|" << endl;
	
	// ve khung tim kiem
	cout << char(218);
	for (int i = 0; i < CR_KTK; i++) {
		cout << char(196);
	}
	cout << char(191) << endl;
	
	cout << char(179) << input;
	for (int i = 0; i < CR_KTK - input.size(); i++) {
		cout << char(255);
	}
	cout << char(179);
	
	cout << endl << char(192);
	for (int i = 0; i < CR_KTK; i++) {
		cout << char(196);
	}
	cout << char(217);
	
	NODEWORD *tuHienTai = inDanhSachTu(tudien, input, pos);
	
	// neu dang o man hinh chinh, dua con tro nhap nhay ve lai khung tim kiem
	gotoxy(1 + input.size(), 5);
	
	return tuHienTai;
}

int demSoTu(hashtable *tudien) {
	int count = 0;
	for (int i = 0; i < 26; i++) {
		if (tudien[i].head == NULL) continue;
		NODEWORD *p = tudien[i].head;
		while (p != NULL) {
			count++;
			p = p->right;
		}
	}
	return count;
}

void xoaTu(hashtable *&tudien, NODEWORD *&p) {
	system("cls");
	
	cout << "Ban co chac muon xoa tu '" << p->info.tu << "' (y/n)?";
	char c = getch();
	if (c != 'y') {
		return;
	}
	
	int index = tapTuDien(p->info.tu);
	
	NODEWORD* head = tudien[index].head;
	NODEWORD* tail = tudien[index].tail;
	
	if (head == tail && head->info.tu == p->info.tu) {
		head = tail = NULL;
	} else if (head->info.tu == p->info.tu) {
		if (head->right == NULL) {
			head = tail = NULL;
		}
		head = head->right;
		head->left = NULL;
	} else if (tail->info.tu == p->info.tu) {
		tail = tail->left;
		tail->right = NULL;
	} else {
		NODEWORD *q = head;
		do {
			q = q->right;
			if (q->info.tu == p->info.tu) {
				NODEWORD *r = q->left;	// nut ben trai nut q
				NODEWORD *s = q->right; // nut ben phai nut q
				
				q->left = NULL;
				q->right = NULL;
				q = NULL;
				delete q;
				
				r->right = s;
				s->left = r;
				break;
			}
		} while(true);
	}
	
	p = NULL;
	delete p;
	tudien[index].head = head;
	tudien[index].tail = tail;
	
	dulieuThaydoi = true;
}

void suaTu(NODEWORD *&p) {
	system("cls");
	cout << "Sua tu: '" << p->info.tu << "':" << endl;
	
	string temp = "";
	cout << "Sua loai tu (" << p->info.loai << "): ";
	getline(cin, temp);
	if (temp == "") return;
	p->info.loai = temp;
	
	for (int i = 0; i < NGHIA_TOI_DA; i++) {
		temp = "";
		cout << "Sua nghia " << i + 1 << " (" << p->info.nghia[i] << "): ";
		getline(cin, temp);
		if (temp == "") break;
		p->info.nghia[i] = temp;
	}
	
	NODEVIDU *vd = p->info.first;
	int count = 0;
	if (vd == NULL) {
		// tu nay chua co vi du nao
		while (true) {
			cout << "Them vi du " << ++count << ": ";
			getline(cin, temp);
			if (temp == "") break;
			chenVdVaoDauDs(p->info.first, temp);
		};
	} else {
		// tu nay da co it nhat 1 vi du
		while (vd != NULL) {
			cout << "Sua vi du " << ++count << " (" << vd->info << "): ";
			getline(cin, temp);
			if (temp == "") break;
			vd->info = temp;
			vd = vd->next;
		}
	}	
	dulieuThaydoi = true;
}

void themTuMoi(hashtable *&tudien) {
	system("cls");
	NODEWORD *p = new NODEWORD();
	p->info.first = NULL;
	
	string temp = "";
	// nhap tu
	cout << "Nhap tu: ";
	getline(cin, temp);
	if (temp == "") return;
	else {
		p->info.tu = temp;
	}
	// nhap loai tu
	temp = "";
	cout << "Nhap loai tu: ";
	getline(cin, temp);
	p->info.loai = temp;
	// nhap toi da 5 nghia
	for (int i = 0; i < NGHIA_TOI_DA; i++) {
		temp = "";
		cout << "Nhap nghia " << i + 1 << ": ";
		getline(cin, temp);
		if (temp == "") break;
		p->info.nghia[i] = temp;
	}
	
	int count = 0;
	while (true) {
		temp = "";
		cout << "Nhap vi du " << ++count << ": ";
		getline(cin, temp);
		if (temp == "") break;
		chenVdVaoDauDs(p->info.first, temp); 
	};
	
	chenTuVaoCuoiTd(tudien, p);
	dulieuThaydoi = true;
}

/* ham xu ly chinh cua chuong trinh */
void xuLyTuDien(hashtable *&tudien) {
	NODEWORD *tuHienTai = NULL;
	string input = ""; // noi dung hien tai cua khung tim kiem
	int keyCode = 0;
	int pos = 0; 
	int soLuongTu = demSoTu(tudien); // dem so luong tu co trong tu dien
	
	while (true) { // vong lap vo tan
		tuHienTai = veGiaoDienChinh(tudien, input, pos);	// ve toan bo giao dien
		keyCode = getch(); // tam dung chuong trinh, nhan ky tu nhap vao		
		switch (keyCode) {
			case 8: // BACKSPACE
				input = input.substr(0, input.size() - 1); // xoa ky tu cuoi
				break;
			case 9: // TAB
				themTuMoi(tudien);
				break;
			case 13: // ENTER
				// vao man hinh chi tiet tu
				veGiaoDienChiTietTu(tuHienTai);
				int k;
				do {
					k = getch();
					if (k == 8) { // BACKSPACE
						// xoa tu nay
						xoaTu(tudien, tuHienTai);
						input = ""; pos = 0;
						break; // ve man hinh chinh
					}
					if (k == 9) { // TAB
						// sua tu nay
						suaTu(tuHienTai);
						input = ""; pos = 0;
						break; // ve man hinh chinh
					}
					if (k == 27) { // ESCAPE
						input = ""; // reset gia tri trong khung tim kiem
						pos = 0;
						break; // ve man hinh chinh
					}
				} while (true);
				break;
			case 27: // ESCAPE
				gotoxy(2, 25);
				if (dulieuThaydoi == true) { // neu du lieu co thay doi
					cout << "Du lieu thay doi!!! Tien hanh ghi file..." << endl;
					ghiFile(tudien);
				}
				exit(EXIT_SUCCESS); // thoat chuong trinh
				break;
			case 224: {
				int key = getch();
				if (key == 72) { pos--; } // UP
				if (key == 80) { pos++; } // DOWN
				if (pos < 0) pos = 0;
				if (pos > soLuongTu - 1) pos = soLuongTu - 1;
				break;
			}
			default:
				if (keyCode >= 97 && keyCode <= 122) { // a-z
					input += char(keyCode);
				}
		}
	};
}	

int main() {	
	// khoi tao tu dien
	hashtable *tudien = new hashtable[26]; //26 chu cai tieng anh
	for (int i = 0; i < 26; i++) {
		tudien[i].head = NULL;
		tudien[i].tail = NULL;
	}
	// doc tap tin tu file data va ghi vao danh sach lien ket
	docFile(tudien);
	xuLyTuDien(tudien);	
	return 0;
}
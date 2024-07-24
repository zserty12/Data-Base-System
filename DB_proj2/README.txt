Project #2 
TYPE 1~7까지의 쿼리문을 수행하는 sql문을 작성함
프로그램을 실행시킨 후 1~7, 그리고 0의 선택지 중 원하는 선택지의 번호를 입력하여
각 TYPE에 대한 정보를 얻을 수 있음
TYPE 1, 2, 3, 4의 경우 SUBTYPE이 있어 main 쿼리에 대한 정보를 받으면 
submenu로 넘어가게 되고 TYPE 1, 2의 경우 subquery가 한 개씩이므로 1을 입력하면 
해당 정보를 얻고 main menu로 자동으로 나가게 되고 TYPE 3, 4의 경우 subquery가 2개씩
있으므로 1, 2, 0의 선택지가 있어 1과 2는 TYPE 3, 4의 각 subquery에 대한 정보를 주고,
0은 다시 main menu로 갈 수 있음
마지막으로 모든 쿼리를 마치면 main menu에서 0을 눌러 프로그램 종료

Snake Game (C++ / ncurses)
────────────────────────────

🎮 프로젝트 개요
C++ 기반 콘솔 환경에서 제작한 Snake Game입니다.
실시간 입력 처리와 충돌 판정, 일정한 프레임 속도 제어를 통해
콘솔에서도 부드럽게 동작하는 게임 구조를 구현했습니다.
게임 로직을 직접 설계하고, ncurses 라이브러리를 활용하여
터미널 환경에서도 시각적인 움직임을 표현할 수 있도록 구성했습니다.

🧩 개발 환경 및 기술 스택
• 언어 : C++17
• 라이브러리 : ncurses 6.5
• 개발 환경 : macOS / Linux (터미널 기반)
• 빌드 도구 : g++

📁 프로젝트 구조
snake_game.project
│
├ main.cpp → 메인 루프 및 초기화
├ map.cpp → 맵 생성과 출력 로직
├ snake.cpp → Snake 이동 및 충돌 처리
├ snake.h → Snake 클래스 정의
├ ncurses-6.5/ → ncurses 라이브러리 폴더
├ main → macOS 실행 파일
├ snake_game → Linux 실행 파일
└ README.md → 프로젝트 설명 파일

⚙️ 주요 기능

실시간 입력 처리

getch()를 이용해 입력 지연 없이 방향 전환

반대 방향 즉시 입력 방지로 부드러운 이동 구현

충돌 판정

Snake 머리가 벽이나 몸과 닿으면 게임 종료

좌표 벡터를 매 프레임 갱신하여 효율적인 검사 수행

게임 루프 구조

일정한 속도로 입력 → 갱신 → 렌더링 순환

코드 구조 예시

while (true) {
    input();
    update();
    render();
}


아이템 및 점수 시스템

랜덤 위치에 음식 생성

Snake가 먹이를 먹으면 길이 증가 및 점수 1점 추가

🚀 실행 방법

(1) ncurses 설치 (Ubuntu 기준)
sudo apt-get install libncurses5-dev libncursesw5-dev

(2) 빌드
g++ main.cpp map.cpp snake.cpp -lncurses -o snake_game

(3) 실행
./snake_game

※ Windows 환경에서는 WSL(Ubuntu)에서 실행하는 것을 권장합니다.

🖼️ 게임 예시 화면
• 시작 화면 → Snake 이동 → 충돌 시 게임 종료
(게임 캡처 이미지는 assets/screenshots/ 디렉터리에 추가 예정입니다.)

💡 개선 방향
• 난이도 조절 기능 추가 (속도 증가, 맵 크기 확장)
• 점수 저장 및 순위 시스템 구현
• 클래스 구조 리팩터링 (GameManager, Renderer 분리)
• SFML 기반 그래픽 버전으로 확장 계획

👨‍💻 개발자 정보
이름 : 이지수
역할 : Snake 이동 로직, 충돌 판정, 게임 루프 구조 설계
GitHub : github.com/kkomakk

🎯 프로젝트 의의
이 프로젝트는 콘솔 환경에서도 실시간 처리가 가능한 게임 루프 구조를 구현하며,
C++의 객체지향 설계와 입력 처리 메커니즘을 학습하기 위해 제작되었습니다.
간단한 게임이지만, 프로그램의 흐름 제어와 시스템 간 데이터 일관성 유지 등
기초적인 구조 설계를 직접 경험할 수 있었습니다.
C++의 객체지향 설계와 입력 처리 메커니즘을 학습하기 위해 제작되었습니다.
간단한 게임이지만, 프로그램의 흐름 제어와 시스템 간 데이터 일관성 유지 등
기초적인 구조 설계를 직접 경험할 수 있었습니다.

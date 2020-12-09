17. Cube를 그리다 : 인덱스 버퍼를 이해하다
- TestVertex1 에 CCube 추가하다
- EarthEngine 에 CCube 관련 코드를 추가하다


16. Axis을 그리다. -> 그리고 우측 마우스 클릭 드래그로 카메라를 회전하다.

- 일리히트 엔진 을 참고하여, 필요한 소스를 옮기다
- 일리히트 엔진 다운로드 주소 : http://irrlicht.sourceforge.net/features/#platforms
- 일리히트 엔진에서 가져온 소스는 include 폴더에 모두 넣어 놨음
- 내가 짠 소스는 source 폴더에 모두 넣어 놨음
- aabbox3d : 바운드박스
- CIndexBuffer : 인덱스 버퍼
- CVertexBuffer : 버텍스 버퍼
- demension2d : width, height 를 갖는 ...
- EarthAllocator :
- EarthArray :
- EarthCompileConfig : 컴파일 옵션
- EarthMath : 수학
- EarthString :
- EarthTypes : int 대신에 s32 를 쓴다. float 대신에 f32 를 쓴다....등등등
- EHardwareBufferFlags :
- heapsort :
- IIndexBuffer : 인덱스 버퍼의 인터페이스
- IReferenceCounted : 레퍼런스 카운트(참조 카운트) - 생성된 객체를 참조할때
                             grab 하고, 참조하지 않을때 drop 한다.
                             count(참조카운터)가 0 이면 delete 한다.
- IVertexBuffer : 버택스버퍼 인터페이스
- line2d :
- line3d
- matrix4 : 행렬
- plane3d : 평면
- position2d : 위치
- quaternion : 쿼터니온 - 회전시 사용함
- rect : 
- S3DVertex :
- SColor :
- SVertexIndex :
- vector2d : 벡터2
- vector3d : 벡터3
** 이상의 참고하여 옮긴 소스는 앞으로 필요할 것으로 예상되어, 옮겨 놓았음.

CAxis : TestVertex1 에 추가했음 (축을 그림)
CCamera : 카메라의 Eye Point 를 마우스 우클릭 드래그로 회전 할 수 있도록 코딩 하였는데, 차후
          좀 더 개선 해야 함.
          - 현재는 마우스와 함께 구현 해 놓았는데, 차후 CMouse 를 만들어서 분리 할 수도 있음.
EarthEngine : main 에 구현 했던 것들을 여기로 옮김
ConsoleApplication1.cpp : 차후에 main.cpp 로 바꿔야 할 것 같음.
CSceneManager : 차후에 CAxis 나, CCamera 등 씬에 관련된 클래스들을 여기로 옮길 예정




15. 텍스쳐를 입히다.

struct SVertex3DT
class CCylinder3DT
추가
핵심포인트
버텍스체 UV 좌표를 설정하고,
텍스쳐를 읽어들여, 맵핑한다.


14.

13.

12.

11.

10.

9.

8.

7.

6.

5.

4.

3.

2.

1.
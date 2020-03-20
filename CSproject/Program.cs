using System;

namespace CSproject
{
    class Program
    {
        public class Student{
            public int m_nNumber;
            public int m_nScore;
            public int m_nRank;
             public Student(int nNumber, int nScore, int nRank = 0){
                 m_nNumber = nNumber;
                 m_nScore = nScore;
                 m_nRank = nRank;
             }

             public void getRank(int nRank){
                 m_nRank = nRank;
             }
        }
        static void Main(string[] args)
        {
            Array arrRecord = Array.CreateInstance(typeof(Student), 5);

            Console.WriteLine("5명의 학생의 성적을 입력하세요.");

            for(int ii = 0; ii < arrRecord.Length; ii++){
                string strScore = Console.ReadLine();
                arrRecord.SetValue(new Student(ii + 1, Convert.ToInt32(strScore)), ii);
            }

            int curRank = 1;
            int i = 0;
            
            while(i < arrRecord.Length){
                int overlapCount = 0;
                Console.WriteLine("i {0} arryRecord: {1}", i, arrRecord.Length); 
                if(((Student)arrRecord.GetValue(i)).m_nRank == 0){
                    int tmpScore = ((Student)arrRecord.GetValue(i)).m_nScore;
                    
                    for(int j = 0; j < arrRecord.Length; j++){
                        if(tmpScore < ((Student)arrRecord.GetValue(j)).m_nScore && ((Student)arrRecord.GetValue(j)).m_nRank == 0)
                            //랭크가 정해지지 않은 것들 중에서 가장 높은 점수 찾는다.
                            tmpScore = ((Student)arrRecord.GetValue(j)).m_nScore;
                            Console.WriteLine("Current Big Score: {0}", tmpScore);
                    }

                    for(int j = 0; j < arrRecord.Length; j++){
                        if(tmpScore == ((Student)arrRecord.GetValue(j)).m_nScore && ((Student)arrRecord.GetValue(j)).m_nRank == 0){
                            //((Student)arrRecord.GetValue(j)).getRank(curRank);
                            ((Student)arrRecord.GetValue(j)).m_nRank = curRank;
                            overlapCount++;
                        }
                    }
                    curRank += overlapCount;
                    for(int j = 0; j < arrRecord.Length; j++){
                        //순위가 정해지지 않은 학생이 있는 경우 다시 처음부터 검색
                        if(((Student)arrRecord.GetValue(j)).m_nRank != 0){
                            i = 0;
                            continue;
                        }
                    }
                }
                i++;
            }

            for(int ii = 0; ii < arrRecord.Length; ii++){
                int nNumber = ((Student)arrRecord.GetValue(ii)).m_nNumber;
                int nScore = ((Student)arrRecord.GetValue(ii)).m_nScore;
                int nRank = ((Student)arrRecord.GetValue(ii)).m_nRank;
                string strGrade = "F";

                if(nScore >= 90)
                    strGrade = "A";
                else if(nScore >= 80)
                    strGrade = "B";
                else if(nScore >= 70)   
                    strGrade = "C";
                else if(nScore >= 60)
                    strGrade = "D";
                
                Console.WriteLine("{0}번학생 성적: {1}   등급: {2}  등 수: {3}", nNumber, nScore, strGrade, nRank);
            }
        }
    }
}

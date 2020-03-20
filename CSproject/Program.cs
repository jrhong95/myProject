using System;

namespace CSproject
{
    class Program
    {
        public class Student{
            public int m_nNumber;
            public int m_nScore;
            public int m_nRank;
            public string m_nGrade;
            public Student(int nNumber, int nScore, int nRank = 0, string nGrade = null){
                 m_nNumber = nNumber;
                 m_nScore  = nScore;
                 m_nRank   = nRank;
                 m_nGrade  = nGrade;
             }
        }

        static void Main(string[] args)
        {
            Array arrRecord = Array.CreateInstance(typeof(Student), 5);
            int studentNumber = arrRecord.Length;
            Console.WriteLine("5명의 학생의 성적을 입력하세요.");

            for (int i = 0; i < studentNumber; i++)
            {
                string strScore = Console.ReadLine();
                arrRecord.SetValue(new Student(i + 1, Convert.ToInt32(strScore)), i);
            }

            //순서대로정렬
            for (int i = 0; i < studentNumber; ++i)
            {

                //for (int j = 0; j < studentNumber; j++)
                //{
                //    Console.WriteLine("{0}등  {1}번 학생, {2}점, {3}", ((Student)arrRecord.GetValue(j)).m_nRank,
                //        ((Student)arrRecord.GetValue(j)).m_nNumber,
                //        ((Student)arrRecord.GetValue(j)).m_nScore,
                //        ((Student)arrRecord.GetValue(j)).m_nGrade);
                //}
                //Console.WriteLine("{0}=======================", i);

                int curScore = ((Student)arrRecord.GetValue(i)).m_nScore;

                if (i < studentNumber - 1)
                {
                    int nextScore = ((Student)arrRecord.GetValue(i + 1)).m_nScore;
                    if (curScore < nextScore)
                    {
                        int tNumber = ((Student)arrRecord.GetValue(i)).m_nNumber;
                        int tScore = ((Student)arrRecord.GetValue(i)).m_nScore;
                        int tRank = ((Student)arrRecord.GetValue(i)).m_nRank;
                        string tGrade = ((Student)arrRecord.GetValue(i)).m_nGrade;

                        //Console.WriteLine("TEMP!!!!!! {0}등  {1}번 학생, {2}점, {3}", tRank, tNumber, tScore, tGrade);
 
                        ((Student)arrRecord.GetValue(i)).m_nNumber = ((Student)arrRecord.GetValue(i + 1)).m_nNumber;
                        ((Student)arrRecord.GetValue(i)).m_nScore =  ((Student)arrRecord.GetValue(i + 1)).m_nScore;
                        ((Student)arrRecord.GetValue(i)).m_nRank =   ((Student)arrRecord.GetValue(i + 1)).m_nRank;
                        ((Student)arrRecord.GetValue(i)).m_nGrade =  ((Student)arrRecord.GetValue(i + 1)).m_nGrade;

                        ((Student)arrRecord.GetValue(i + 1)).m_nNumber = tNumber;
                        ((Student)arrRecord.GetValue(i + 1)).m_nScore = tScore;
                        ((Student)arrRecord.GetValue(i + 1)).m_nRank = tRank;
                        ((Student)arrRecord.GetValue(i + 1)).m_nGrade = tGrade;

                        i = -1;
                    }
                }
            }
           
            int overlapCount = 0;
            int curRank = 1;
            for (int i = 0; i < studentNumber; i++)
            {
                if (i == 0) {
                    //Console.WriteLine("{0} Done.", i);
                    ((Student)arrRecord.GetValue(i)).m_nRank = curRank;
                    overlapCount++; 
                }
                else if (i != 0 && ((Student)arrRecord.GetValue(i - 1)).m_nScore == ((Student)arrRecord.GetValue(i)).m_nScore)
                {
                    //Console.WriteLine("{0} Done.", i);
                    ((Student)arrRecord.GetValue(i)).m_nRank = curRank;
                    overlapCount++;
                }
                else
                {
                    //Console.WriteLine("{0} Done.", i);
                    curRank += overlapCount;
                    overlapCount = 0;
                    ((Student)arrRecord.GetValue(i)).m_nRank = curRank;
                    overlapCount++;
                }
                    
            }
            //Console.WriteLine("dddddddddddddddddd");
            //for (int j = 0; j < studentNumber; j++)
            //{
            //    Console.WriteLine("{0}등  {1}번 학생, {2}점, {3}", ((Student)arrRecord.GetValue(j)).m_nRank,
            //        ((Student)arrRecord.GetValue(j)).m_nNumber,
            //        ((Student)arrRecord.GetValue(j)).m_nScore,
            //        ((Student)arrRecord.GetValue(j)).m_nGrade);
            //}
            //등급 저장
            for (int i = 0; i < studentNumber; i++)
            {
                int nScore = ((Student)arrRecord.GetValue(i)).m_nScore;
                string strGrade = "F";

                if (nScore >= 90)
                    strGrade = "A";
                else if (nScore >= 80)
                    strGrade = "B";
                else if (nScore >= 70)
                    strGrade = "C";
                else if (nScore >= 60)
                    strGrade = "D";

                ((Student)arrRecord.GetValue(i)).m_nGrade = strGrade;
            }

            for(int n = 0; n < studentNumber; n++){

                for(int i = 0; i < studentNumber; i++) {
                    int nRank = ((Student)arrRecord.GetValue(i)).m_nRank;
                    int nNumber = ((Student)arrRecord.GetValue(i)).m_nNumber;
                    int nScore = ((Student)arrRecord.GetValue(i)).m_nScore;
                    string nGrade = ((Student)arrRecord.GetValue(i)).m_nGrade;

                    if (nRank == n + 1){
                        Console.WriteLine("{0}등  {1}번 학생, {2}점, {3}", nRank, nNumber, nScore, nGrade);
                    }
                }
            }
        }
    }
}

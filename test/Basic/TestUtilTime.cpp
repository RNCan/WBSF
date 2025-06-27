#include <iostream>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/csv_oarchive.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

#include <catch2/catch_all.hpp>

#include "Basic/UtilTime.h"


using namespace std;
using namespace WBSF;


TEST_CASE( "UtilTime", "[global]" )
{
    SECTION( "GetMonthName" )
    {
        REQUIRE( strcmp(GetMonthName(JANUARY, true), "January" ) == 0);
        REQUIRE( strcmp(GetMonthName(JANUARY, false), "Jan" )== 0);
        REQUIRE( strcmp(GetMonthTitle(DECEMBER, true), "December" )== 0);
        REQUIRE( strcmp(GetMonthTitle(DECEMBER, false), "Dec" )== 0);
    }

    SECTION( "IsLeap" )
    {
        REQUIRE( IsLeap(YEAR_NOT_INIT) == false);
        REQUIRE( IsLeap(-998) == false);
        REQUIRE( IsLeap(-4) == false );
        REQUIRE( IsLeap(-1) == false );
        REQUIRE( IsLeap(0) == false );
        REQUIRE( IsLeap(1600) == true );
        REQUIRE( IsLeap(1700) == false );
        REQUIRE( IsLeap(1800) == false );
        REQUIRE( IsLeap(1900) == false );
        REQUIRE( IsLeap(1904) == true );
        REQUIRE( IsLeap(2000) == true );
        REQUIRE( IsLeap(2001) == false );
    }

    SECTION( "GetNbDays" )
    {
        REQUIRE( GetNbDaysPerYear(YEAR_NOT_INIT) == 365);
        REQUIRE( GetNbDaysPerYear(-4) == 365);
        REQUIRE( GetNbDaysPerYear(0) == 365);
        REQUIRE( GetNbDaysPerYear(4) == 366);
        REQUIRE( GetNbDaysPerYear(1600) == 366);
        REQUIRE( GetNbDaysPerYear(1900) == 365);
        REQUIRE( GetNbDaysPerYear(2000) == 366);
        REQUIRE( GetNbDaysPerYear(2002) == 365);
        REQUIRE( GetNbDaysPerYear(2004) == 366);
        REQUIRE( GetNbDaysPerYear(2100) == 365);

        REQUIRE( GetNbDaysPerMonth(YEAR_NOT_INIT, JANUARY) == 31);
        REQUIRE( GetNbDaysPerMonth(-4, NOVEMBER) == 30);
        REQUIRE( GetNbDaysPerMonth(0, OCTOBER) == 31);
        REQUIRE( GetNbDaysPerMonth(4, SEPTEMBER) == 30);
        REQUIRE( GetNbDaysPerMonth(2002, AUGUST) == 31);
        REQUIRE( GetNbDaysPerMonth(2004, JULY) == 31);
        REQUIRE( GetNbDaysPerMonth(JUNE) == 30);

        REQUIRE( GetNbDaysPerMonth(-4, FEBRUARY) == 28);
        REQUIRE( GetNbDaysPerMonth(0, FEBRUARY) == 28);
        REQUIRE( GetNbDaysPerMonth(4, FEBRUARY) == 29);
        REQUIRE( GetNbDaysPerMonth(2002, FEBRUARY) == 28);
        REQUIRE( GetNbDaysPerMonth(2004, FEBRUARY) == 29);
        REQUIRE( GetNbDaysPerMonth(FEBRUARY) == 28);


    }

    SECTION( "GetDOY" )
    {
        REQUIRE( GetDOY(2000, FEBRUARY, DAY_29) == 59);
        REQUIRE( GetDOY(2000, MARCH, DAY_01) == 60);
        REQUIRE( GetDOY(2001, MARCH, DAY_01) == 59);
        REQUIRE( GetDOY(2000, APRIL, DAY_15) == 105);


        REQUIRE( GetFirstDOY(JANUARY) == 0);
        REQUIRE( GetLastDOY(DECEMBER) == 364);

        REQUIRE( GetLastDOY(FEBRUARY) == 58);
        REQUIRE( GetFirstDOY(MARCH) == 59);

        REQUIRE( GetLastDOY(2002, FEBRUARY) == 58);
        REQUIRE( GetFirstDOY(2002, MARCH) == 59);

        REQUIRE( GetLastDOY(2000, FEBRUARY) == 59);
        REQUIRE( GetFirstDOY(2000, MARCH) == 60);
        REQUIRE( GetFirstDOY(2000, JULY) == GetDOY(2000, JULY, DAY_01));
        REQUIRE( GetFirstDOY(2000, AUGUST) == GetDOY(2000, AUGUST, DAY_01));
        REQUIRE( GetLastDOY(2004, AUGUST) == GetDOY(2004, AUGUST, DAY_31));


        REQUIRE( GetMonth( 2000, GetDOY(2000, FEBRUARY, DAY_29)) == FEBRUARY);
        REQUIRE( GetMonth( 2000, GetDOY(2000, MARCH, DAY_01)) == MARCH);
        REQUIRE( GetMonth( 1974, GetDOY(1974, APRIL, DAY_15)) == APRIL);
        REQUIRE( GetDayOfTheMonth( 2000, GetDOY(2000, FEBRUARY, DAY_29)) == DAY_29);
        REQUIRE( GetDayOfTheMonth( 2000, GetDOY(2000, MARCH, DAY_01)) == DAY_01);
        REQUIRE( GetDayOfTheMonth( 1974, GetDOY(1974, APRIL, DAY_15)) == DAY_15);
    }



    SECTION( "GetMonth" )
    {
        REQUIRE( GetMonth("blabla") == NOT_INIT);


        int32_t Uref = GetRef(2020, SEPTEMBER, DAY_22);
        REQUIRE( Uref == 737689);

        int year=0;
        size_t m=0;
        size_t d=0;
        SetRef(Uref, year, m, d);

        REQUIRE( year == 2020);
        REQUIRE( m == SEPTEMBER);
        REQUIRE( d == DAY_22);
    }

}

TEST_CASE( "UtilTime", "[CTM]" )
{
    SECTION( "GetTypeName" )
    {

        REQUIRE( strcmp(CTM::GetTypeName(CTM::ANNUAL), "Annual") == 0);
        REQUIRE( strcmp(CTM::GetTypeName(CTM::ATEMPORAL), "Atemporal") == 0);
        REQUIRE( strcmp(CTM::GetModeName(CTM::FOR_EACH_YEAR), "For each year") == 0);
        REQUIRE( strcmp(CTM::GetModeName(CTM::OVERALL_YEARS), "Overall years") == 0);

        REQUIRE( CTM::IsAvailable(CTM(CTM::DAILY), CTM (CTM::MONTHLY)) );
        REQUIRE( !CTM::IsAvailable(CTM(CTM::MONTHLY), CTM (CTM::DAILY)) );
        REQUIRE( CTM::IsAvailable(CTM(CTM::DAILY), CTM (CTM::DAILY, CTM::OVERALL_YEARS)) );


        CTM TM(CTM::MONTHLY, CTM::OVERALL_YEARS);
        REQUIRE( TM.GetTypeName() == "Monthly" );
        REQUIRE( TM.GetModeName() == "Overall years" );
    }

    SECTION( "Operator" )
    {

        CTM TM1(CTM::MONTHLY);
        CTM TM2(CTM::MONTHLY, CTM::OVERALL_YEARS);
        CTM TM3(CTM::ANNUAL, CTM::FOR_EACH_YEAR);

        REQUIRE( TM1 !=  TM2);
        REQUIRE( TM1 !=  TM3);

        TM1.Type( CTM::ANNUAL );
        REQUIRE( TM1 ==  TM3);
    }



}

TEST_CASE( "UtilTime", "[CTRefFormat]" )
{
    REQUIRE( strcmp(CTRefFormat::GetDefaultFormat(CTM::DAILY), "%Y-%m-%d") == 0);
}


TEST_CASE( "UtilTime", "[CTRef]" )
{
    SECTION( "GetMonthName" )
    {
//
//        struct STest
//        {
//            int32_t m_year : 16;   // -65535..65536  (16 bits)
//            uint32_t m_month : 4;    // 0..16 (4 bits)
//            uint32_t m_day : 5;    // 0..31 (5 bits)
//            uint32_t m_hour : 5;    // 0..31 (5 bits)
//            uint32_t m_mode: 2;    // 0..4 (2 bits)
//        };
//
//        STest test1 = {2005, JULY, DAY_15, 0, 0};
//        STest test2 = {2005, OCTOBER, DAY_12, 0, 0};
//        //bool brep = int32_t( *(void*)(&test1) ) < int32_t( *(void*)(&test2));
//
//        const void* p_bit1 = &test1;
//        int32_t i1 = *((int32_t*) p_bit1);
//
//        void* p_bit2 = &test2;
//        int32_t i2 = *((int32_t*) p_bit2);
//
//        REQUIRE( i1 < i2 );
    }

    SECTION( "is_valid" )
    {
        REQUIRE( CTRef::is_valid(0, 0, 0, 0, CTM::ANNUAL) );
        REQUIRE( CTRef::is_valid(2000, 0, 0, 0, CTM::ANNUAL) );
        REQUIRE( CTRef::is_valid(YEAR_NOT_INIT, 0, 0, 0, CTM::ANNUAL, CTM::OVERALL_YEARS) );
        REQUIRE( CTRef::is_valid(2000, JANUARY, 0, 0, CTM::MONTHLY) );
        REQUIRE( CTRef::is_valid(YEAR_NOT_INIT, JANUARY, 0, 0, CTM::MONTHLY, CTM::OVERALL_YEARS) );
        REQUIRE( CTRef::is_valid(2000, JANUARY, DAY_01, 0, CTM::DAILY) );
        REQUIRE( CTRef::is_valid(YEAR_NOT_INIT, JANUARY, DAY_01, 0, CTM::DAILY, CTM::OVERALL_YEARS) );
        REQUIRE( CTRef::is_valid(2000, FEBRUARY, DAY_29, 0, CTM::DAILY) );
        REQUIRE( CTRef::is_valid(2000, JANUARY, DAY_01, 0, CTM::HOURLY) );
        REQUIRE( CTRef::is_valid(YEAR_NOT_INIT, JANUARY, DAY_01, 0, CTM::HOURLY, CTM::OVERALL_YEARS) );
        REQUIRE( CTRef::is_valid(412, 0, 0, 0, CTM::ATEMPORAL) );


        REQUIRE( !CTRef::is_valid(2000, DECEMBER, DAY_31, 25, CTM::HOURLY) );
        REQUIRE( !CTRef::is_valid(2000, DECEMBER, DAY_31, -1, CTM::HOURLY) );
        REQUIRE( !CTRef::is_valid(2000, JANUARY, DAY_01-1, 0, CTM::DAILY) );
        REQUIRE( !CTRef::is_valid(2000, DECEMBER, DAY_31+1, 0, CTM::DAILY) );
        REQUIRE( !CTRef::is_valid(YEAR_NOT_INIT, FEBRUARY, DAY_29, 0, CTM::DAILY, CTM::OVERALL_YEARS) );
        REQUIRE( !CTRef::is_valid(2001, FEBRUARY, DAY_29, 0, CTM::DAILY) );
        REQUIRE( !CTRef::is_valid(2000, JANUARY-1, 0, 0, CTM::MONTHLY) );
        REQUIRE( !CTRef::is_valid(2000, DECEMBER+1, 0, 0, CTM::MONTHLY) );
        REQUIRE( !CTRef::is_valid(4000, 0, 0, 0, CTM::ANNUAL) );
        REQUIRE( !CTRef::is_valid(0, 0, 0, 0, CTM::ATEMPORAL, CTM::OVERALL_REF) );


    }



    SECTION( "Constructor" )
    {
        //cout << "Constructor" << endl;
        CTRef TRefA1(2010);
        REQUIRE( TRefA1.TM().Type() == CTM::ANNUAL );
        REQUIRE( TRefA1.TM().Mode() == CTM::FOR_EACH_YEAR);
        REQUIRE( TRefA1.m_year == 2010 );


        CTRef TRefM1(2022, MARCH);
        REQUIRE( TRefM1.TM().Type() == CTM::MONTHLY );
        REQUIRE( TRefM1.TM().Mode() == CTM::FOR_EACH_YEAR);
        REQUIRE( TRefM1.m_year == 2022 );
        REQUIRE( TRefM1.m_month == MARCH);


        CTRef TRefD1(2004, FEBRUARY, DAY_29);
        REQUIRE( TRefD1.TM().Type() == CTM::DAILY );
        REQUIRE( TRefD1.TM().Mode() == CTM::FOR_EACH_YEAR);
        REQUIRE( TRefD1.m_year == 2004 );
        REQUIRE( TRefD1.m_month == FEBRUARY);
        REQUIRE( TRefD1.m_day == DAY_29);

        CTRef TRefH1(2004, FEBRUARY, DAY_29, 16ull );
        REQUIRE( TRefH1.TM().Type() == CTM::HOURLY );
        REQUIRE( TRefH1.TM().Mode() == CTM::FOR_EACH_YEAR);
        REQUIRE( TRefH1.m_year == 2004 );
        REQUIRE( TRefH1.m_month == FEBRUARY);
        REQUIRE( TRefH1.m_day == DAY_29);
        REQUIRE( TRefH1.m_hour == 16);

        CTRef TRefR1(12345, 0, 0, 0, CTM::ATEMPORAL);
        REQUIRE( TRefR1.TM().Type() == CTM::ATEMPORAL );
        REQUIRE( TRefR1.TM().Mode() == CTM::FOR_EACH_REF);
        REQUIRE( TRefR1.m_ref == 12345);

        CTRef TRefD2("2004-06-07");
        REQUIRE( TRefD2.TM().Type() == CTM::DAILY );
        REQUIRE( TRefD2.TM().Mode() == CTM::FOR_EACH_YEAR);
        REQUIRE( TRefD2.m_year == 2004 );
        REQUIRE( TRefD2.m_month == JUNE);
        REQUIRE( TRefD2.m_day == DAY_07);

        CTRef TRefH2("1804-11-22 18:00:00");
        REQUIRE( TRefH2.TM().Type() == CTM::HOURLY );
        REQUIRE( TRefH2.TM().Mode() == CTM::FOR_EACH_YEAR);
        REQUIRE( TRefH2.m_year == 1804 );
        REQUIRE( TRefH2.m_month == NOVEMBER);
        REQUIRE( TRefH2.m_day == DAY_22);
        REQUIRE( TRefH2.m_hour == 18);


        CTRef TRefA2("01");
        REQUIRE( TRefA2.TM().Type() == CTM::ANNUAL );
        REQUIRE( TRefA2.TM().Mode() == CTM::FOR_EACH_YEAR);
        REQUIRE( TRefA2.m_year == 1 );


        CTRef TRefM2("9999-12");
        REQUIRE( TRefM2.TM().Type() == CTM::MONTHLY );
        REQUIRE( TRefM2.TM().Mode() == CTM::OVERALL_YEARS);
        REQUIRE( TRefM2.m_month == DECEMBER);

        CTRef TRefA3("9999");
        REQUIRE( TRefA3.TM().Type() == CTM::ANNUAL );
        REQUIRE( TRefA3.TM().Mode() == CTM::OVERALL_YEARS);

        CTRef TRef;
        REQUIRE( !TRef.is_init() );
        TRef = CTRef(2010, SEPTEMBER, DAY_12);
        REQUIRE( TRef.is_init() );
        REQUIRE( TRef.is_valid() );
        TRef.clear();
        REQUIRE( !TRef.is_init() );
        REQUIRE( TRef.is_valid() );
    }

    SECTION( "Get/Set" )
    {
        CTRef TRef;

        TRef.Set(2020, OCTOBER, DAY_20, 0, CTM::DAILY);
        REQUIRE( TRef ==  CTRef("2020-10-20") );

        REQUIRE( TRef.GetDOY() == GetDOY(2020, OCTOBER, DAY_20) );

        TRef.SetDOY( 2020, GetDOY(2020, OCTOBER, DAY_21) );
        REQUIRE( TRef ==  CTRef("2020-10-21") );
    }

    SECTION( "Operator" )
    {
        CTRef TRef1("2010-12-25");
        CTRef TRef2("2011-11-11");

        REQUIRE( TRef2 != TRef1 );
        REQUIRE( TRef2 > TRef1 );
        REQUIRE( TRef2 >= TRef1 );
        REQUIRE( TRef1 < TRef2 );
        REQUIRE( TRef1 <= TRef2 );


        TRef2 = TRef1;
        REQUIRE( TRef2 == TRef1 );
        REQUIRE( TRef2 >= TRef1 );
        REQUIRE( TRef2 <= TRef1 );

        TRef2--;
        REQUIRE( TRef2 == CTRef("2010-12-24") );

        REQUIRE( TRef2 - TRef1 == -1 );
        REQUIRE( TRef1 - TRef2 == 1 );

        TRef2++;
        REQUIRE( TRef2 == CTRef("2010-12-25") );


        TRef2 += 365;
        REQUIRE( TRef2 == CTRef("2011-12-25") );

        CTRef TRef3(2011,JANUARY);
        TRef3 -= 2;
        REQUIRE( TRef3 == CTRef("2010-11") );

        REQUIRE( TRef3 + 12 == CTRef("2011-11") );
        REQUIRE( TRef3 - 12 == CTRef("2009-11") );
    }

    SECTION( "as" )
    {
        CTRef TRef("1111-11-11 11:00:00");
        REQUIRE( TRef.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR)) == CTRef("1111-11-11 11:00:00") );
        REQUIRE( TRef.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS)) == CTRef("9999-11-11 11:00:00") );
        REQUIRE( TRef.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR)) == CTRef("1111-11-11") );
        REQUIRE( TRef.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS)) == CTRef("9999-11-11") );
        REQUIRE( TRef.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR)) == CTRef("1111-11") );
        REQUIRE( TRef.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS)) == CTRef("9999-11") );
        REQUIRE( TRef.as(CTM(CTM::ANNUAL, CTM::FOR_EACH_YEAR)) == CTRef("1111") );
        REQUIRE( TRef.as(CTM(CTM::ANNUAL, CTM::OVERALL_YEARS)) == CTRef("9999") );

        TRef = CTRef ("1111");
        REQUIRE( TRef.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR), CTM::FIRST_TREF) == CTRef("1111-01") );
        REQUIRE( TRef.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR), CTM::MID_TREF) == CTRef("1111-06") );
        REQUIRE( TRef.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR), CTM::LAST_TREF) == CTRef("1111-12") );
        REQUIRE( TRef.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS), CTM::FIRST_TREF) == CTRef("9999-01") );
        REQUIRE( TRef.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS), CTM::MID_TREF) == CTRef("9999-06") );
        REQUIRE( TRef.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS), CTM::LAST_TREF) == CTRef("9999-12") );

        TRef = CTRef ("1111-11");
        REQUIRE( TRef.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR), CTM::FIRST_TREF) == CTRef("1111-11-01") );
        REQUIRE( TRef.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR), CTM::MID_TREF) == CTRef("1111-11-15") );
        REQUIRE( TRef.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR), CTM::LAST_TREF) == CTRef("1111-11-30") );
        REQUIRE( TRef.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS), CTM::FIRST_TREF) == CTRef("9999-11-01") );
        REQUIRE( TRef.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS), CTM::MID_TREF) == CTRef("9999-11-15") );
        REQUIRE( TRef.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS), CTM::LAST_TREF) == CTRef("9999-11-30") );

        TRef = CTRef ("1111-11-11");
        REQUIRE( TRef.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR), CTM::FIRST_TREF) == CTRef("1111-11-11 00:00:00") );
        REQUIRE( TRef.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR), CTM::MID_TREF) == CTRef("1111-11-11 11:00:00") );
        REQUIRE( TRef.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR), CTM::LAST_TREF) == CTRef("1111-11-11 23:00:00") );
        REQUIRE( TRef.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS), CTM::FIRST_TREF) == CTRef("9999-11-11 00:00:00") );
        REQUIRE( TRef.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS), CTM::MID_TREF) == CTRef("9999-11-11 11:00:00") );
        REQUIRE( TRef.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS), CTM::LAST_TREF) == CTRef("9999-11-11 23:00:00") );
    }

    SECTION( "Temporal/Atemporal" )
    {
        REQUIRE( CTRef("2008-11-11 23:00:00").IsTemporal() );
        REQUIRE( !CTRef("9999-11").IsAtemporal() );
        REQUIRE( CARef(0).IsAtemporal() );
    }

    SECTION( "string" )
    {
        CTRef Tref("2024-10-16");
        REQUIRE( Tref == CTRef(2024, OCTOBER, DAY_16) );
        REQUIRE( Tref.to_string() == "2024-10-16" );

        Tref = CTRef ("2024-10-16 04:11:00");
        REQUIRE( Tref == CTRef(2024, OCTOBER, DAY_16, 4) );
        REQUIRE( Tref.to_string() == "2024-10-16 04:00:00" );//minutes, seconds not supported
    }

    SECTION( "serialize" )
    {
        CTRef Tref1("2024-10-16");

        try
        {
            //, ios::binary
            std::ofstream ofs("E:/Project(GitHubDesktop)/WBSFMultiPlatform/test/Data/filename.xml");


            //unsigned int flags = boost::archive::no_header;
            //boost::archive::xml_oarchive oa(ofs);
            boost::archive::text_oarchive oa(ofs);
            oa << boost::serialization::make_nvp("CTRef", Tref1);
            //oa & boost::serialization::make_nvp("TM", Tref1->TM());

            //boost::archive::text_oarchive oa(ofs, flags );
            //oa << Tref;

            ofs.close();


        }
        catch(boost::archive::archive_exception& e)
        {
            cout << "Error writing: " << e.what() << endl;
        }
        catch(...)
        {
            cout << "Error writing " << endl;
        }

        CTRef Tref2;
        try
        {
            //, ios::binary
            std::ifstream ifs("E:/Project(GitHubDesktop)/WBSFMultiPlatform/test/Data/filename.xml");
            //boost::archive::xml_iarchive ia(ifs);
            boost::archive::text_iarchive ia(ifs);
            ia >> BOOST_SERIALIZATION_NVP(Tref2);
            //ia & boost::serialization::make_nvp("TM", TM1);
            ifs.close();
        }
        catch(boost::archive::archive_exception& e)
        {
            cout << "Error reading: " << e.what() << endl;
        }
        catch(...)
        {
            cout << "Error reading " << endl;
        }

        REQUIRE( Tref2 == Tref1 );

    }
}


TEST_CASE( "UtilTime", "[CDOYRef]" )
{
    SECTION( "Constructor" )
    {
        CTRef Tref("2019-09-09");
        CDOYRef DOYref(Tref.GetYear(), Tref.GetDOY());

        REQUIRE( DOYref == Tref );
    }
}



TEST_CASE( "UtilTime", "[CTimeRef]" )
{
    SECTION( "Constructor" )
    {
        CTRef Tref1("2019-09-09");
        __time64_t time64 = Tref1.get_time64();

        CTimeRef Tref2(time64);

        REQUIRE( Tref2 == Tref1 );
    }
}


TEST_CASE( "UtilTime", "[CTPeriod]" )
{

    SECTION( "is_valid" )
    {
        REQUIRE( CTPeriod::is_valid(CTRef("2024-10-16"), CTRef("2024-10-16")) );
        REQUIRE( CTPeriod::is_valid(CTRef("2024-10-16"), CTRef("2024-10-20")) );
        REQUIRE( !CTPeriod::is_valid(CTRef("2024-10-20"), CTRef("2024-10-16")) );
        REQUIRE( !CTPeriod::is_valid(CTRef("2024-10-20"), CTRef("2024-11")) );
    }

    SECTION( "string" )
    {
        CTPeriod p("2004-02-29|2004-06-07");
        REQUIRE( p.begin() == CTRef(2004, FEBRUARY, DAY_29) );
        REQUIRE( p.end() == CTRef(2004, JUNE, DAY_07) );
        REQUIRE( p == CTPeriod("[2004-02-29|2004-06-07]") );
        REQUIRE( p == CTPeriod("2004-02-29","2004-06-07") );
        REQUIRE( p == CTPeriod(CTRef("2004-02-29"),CTRef("2004-06-07")) );
    }

    SECTION( "Constructor" )
    {
        CTRef TRefA1(2010);
        CTRef TRefA2("2020");

        CTRef TRefM1(2022, MARCH);
        CTRef TRefM2("2024-12");


        CTRef TRefD1(2004, FEBRUARY, DAY_29);
        CTRef TRefD2("2004-06-07");

        CTRef TRefH1(2004, FEBRUARY, DAY_29, 16 );
        CTRef TRefH2("2004-11-22 18:00:00");

        CTRef TRefR1(12345, 0, 0, 0, CTM::ATEMPORAL);
        CTRef TRefR2(23456, 0, 0, 0, CTM::ATEMPORAL);


        CTPeriod pA1(TRefA1, TRefA2);
        CTPeriod pA2("2010", "2020");
        REQUIRE( pA1.begin() == TRefA1 );
        REQUIRE( pA1.end() == TRefA2 );
        REQUIRE( pA1 == pA2 );
        REQUIRE( pA1.size() == 11 );


        CTPeriod pM1(TRefM1, TRefM2);
        CTPeriod pM2("2022-03", "2024-12");
        REQUIRE( pM1.begin() == TRefM1 );
        REQUIRE( pM1.end() == TRefM2 );
        REQUIRE( pM1 == pM2 );

        CTPeriod pD1(TRefD1, TRefD2);
        CTPeriod pD2("2004-02-29", "2004-06-07");
        REQUIRE( pD1.begin() == TRefD1 );
        REQUIRE( pD1.end() == TRefD2 );
        REQUIRE( pD1 == pD2 );

        CTPeriod pH1(TRefH1, TRefH2);
        CTPeriod pH2("2004-02-29 16:00:00", "2004-11-22 18:00:00");
        REQUIRE( pH1.begin() == TRefH1 );
        REQUIRE( pH1.end() == TRefH2 );
        REQUIRE( pH1 == pH2 );

        CTPeriod pR1(TRefR1, TRefR2);
        CTPeriod pR2(CARef(12345), CARef(23456));
        REQUIRE( pR1.begin() == TRefR1 );
        REQUIRE( pR1.end() == TRefR2 );
        REQUIRE( pR1 == pR2 );

        CTPeriod pH3("[2004-02-29 16:00:00|2004-11-22 18:00:00]");
        REQUIRE( pH3 == pH1 );

        CTPeriod pD3("2004-02-29|2004-06-07");
        REQUIRE( pD3 == pD1 );


        CTPeriod pM3(pM2);
        REQUIRE( pM3 == pM1 );

        pA2 = pA1;
        REQUIRE( pA2 == pA1 );

        pA2.clear();
        REQUIRE( pA2 != pA1 );
        REQUIRE( pA2.size() == 0 );
    }


    SECTION( "Operator" )
    {
        CTPeriod p1(CTRef("2010-12-25"),CTRef("2011-01-11"));
        CTPeriod p2 = p1;

        REQUIRE( p2.is_inside(CTRef("2010-12-31")) );
        REQUIRE( p2.is_inside(p2) );
        REQUIRE( p2.is_intersect(p1) );

        p2 += CTRef("2010-12-31");
        REQUIRE( p2 == p1 );

        REQUIRE( !p2.is_inside(CTRef("2010-12-20")) );
        p2 += CTRef("2010-12-20");
        REQUIRE( p2 != p1 );
        REQUIRE( p2.begin() == CTRef(2010,DECEMBER,DAY_20) );
        REQUIRE( p2.end() == p1.end() );
        REQUIRE( p2.is_intersect(p1) );

        REQUIRE( !p2.is_inside(CTRef("2011-01-12")) );
        p2 += CTRef("2011-01-20");
        REQUIRE( p2.begin() == CTRef(2010,DECEMBER,DAY_20) );
        REQUIRE( p2.end() == CTRef(2011,JANUARY,DAY_20));
        REQUIRE( p2.is_intersect(p1) );

        p2 = p1;

        p2.inflate(CTRef("2010-12-31"));
        REQUIRE( p2 == p1 );

        p2.inflate(CTRef("2010-12-20"));
        REQUIRE( p2 != p1 );
        REQUIRE( p2.begin() == CTRef(2010,DECEMBER,DAY_20) );
        REQUIRE( p2.end() == p1.end() );

        p2.inflate(CTRef("2011-01-20"));
        REQUIRE( p2.begin() == CTRef(2010,DECEMBER,DAY_20) );
        REQUIRE( p2.end() == CTRef(2011,JANUARY,DAY_20));

        REQUIRE( !p2.is_intersect(CTPeriod("2010-01-01|2010-12-19")) );
        REQUIRE( p2.is_intersect(CTPeriod("2010-01-01|2010-12-20")) );
        REQUIRE( p2.is_intersect(CTPeriod("2011-01-20|2011-01-20")) );
        REQUIRE( !p2.is_intersect(CTPeriod("2011-01-21|2011-01-28")) );

        REQUIRE( CTPeriod::unions(CTPeriod("2010-01-01|2010-01-10"), CTPeriod("2010-01-20|2010-01-31")) == CTPeriod("2010-01-01|2010-01-31") );
        REQUIRE( CTPeriod::unions(CTPeriod("2010-01-01|2010-12-31"), CTPeriod("2010-07-01|2010-07-31")) == CTPeriod("2010-01-01|2010-12-31") );
        REQUIRE( CTPeriod::unions(CTPeriod("2010-01-01|2010-01-10"), CTRef("2010-01-31")) == CTPeriod("2010-01-01|2010-01-31") );

        REQUIRE( !CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-10"), CTPeriod("2010-01-20|2010-01-31")).is_init() );
        REQUIRE( !CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-10"), CTRef("2010-01-31")).is_init() );
        REQUIRE( CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-31"), CTPeriod("2010-01-15|2010-02-15")) == CTPeriod("2010-01-15|2010-01-31"));
        REQUIRE( CTPeriod::intersect(CTPeriod("2010-01-15|2010-01-15"), CTPeriod("2010-01-15|2010-01-15")) == CTPeriod("2010-01-15|2010-01-15"));
        REQUIRE( CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-31"), CTPeriod("2010-01-15|2010-01-17")) == CTPeriod("2010-01-15|2010-01-17"));
        REQUIRE( CTPeriod::intersect(CTPeriod("2010-01-15|2010-01-31"), CTPeriod("2010-01-01|2010-01-15")) == CTPeriod("2010-01-15|2010-01-15"));
        REQUIRE( CTPeriod::intersect(CTPeriod("2010-01-01|2010-01-10"), CTRef("2010-01-05")) == CTPeriod("2010-01-05|2010-01-05") );


        p1 += p2;
        REQUIRE( p1 == p2 );

        REQUIRE( p1[4] == CTRef("2010-12-24"));
        REQUIRE( !p1[-1].is_init());
        REQUIRE( !p1[p1.size()+1].is_init());
        REQUIRE( p1[0] == p1.begin());
        REQUIRE( p1[p1.size()-1] == p1.end());


        REQUIRE( p1[CTRef("2010-12-24")] == 4);
        REQUIRE( p1[p1.begin()] == 0);
        REQUIRE( p1[p1.end()] == p1.size()-1);
        REQUIRE( p1[p1.begin()-1] == NOT_INIT);
        REQUIRE( p1[p1.end()+1] == NOT_INIT);
    }

    SECTION( "Others" )
    {
        CTPeriod p1("2000-12","2010-01");

        REQUIRE( p1.GetFirstYear() == 2000);
        REQUIRE( p1.GetLastYear() == 2010);
        REQUIRE( p1.GetNbYears() == 11);
        REQUIRE( p1.TM() == CTM::MONTHLY);

        p1.TM(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS));
        REQUIRE( p1.GetFirstYear() == 9999);
        REQUIRE( p1.GetLastYear() == 9999);
        REQUIRE( p1.GetNbYears() == 1);
        REQUIRE( p1.is_countinious() );
    }

    SECTION( "as" )
    {
        CTPeriod p("1111-11-11 11:00:00", "1212-12-12 12:00:00");

        REQUIRE( p.as(CTM(CTM::HOURLY, CTM::FOR_EACH_YEAR)) == CTPeriod("1111-11-11 11|1212-12-12 12") );
        REQUIRE( p.as(CTM(CTM::HOURLY, CTM::OVERALL_YEARS)) == CTPeriod("9999-11-11 11|9999-12-12 12") );
        REQUIRE( p.as(CTM(CTM::DAILY, CTM::FOR_EACH_YEAR)) == CTPeriod("1111-11-11|1212-12-12") );
        REQUIRE( p.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS)) == CTPeriod("9999-11-11|9999-12-12") );
        REQUIRE( p.as(CTM(CTM::MONTHLY, CTM::FOR_EACH_YEAR)) == CTPeriod("1111-11|1212-12") );
        REQUIRE( p.as(CTM(CTM::MONTHLY, CTM::OVERALL_YEARS)) == CTPeriod("9999-11|9999-12") );
        REQUIRE( p.as(CTM(CTM::ANNUAL, CTM::FOR_EACH_YEAR)) == CTPeriod("1111|1212") );
        REQUIRE( p.as(CTM(CTM::ANNUAL, CTM::OVERALL_YEARS)) == CTPeriod("9999","9999") );

        p = CTPeriod("2000-12-01", "2001-01-31");
        REQUIRE( p.as(CTM(CTM::DAILY, CTM::OVERALL_YEARS)) == CTPeriod("9999-01-31|9999-12-01"));//month is revert
    }



    SECTION( "serialize" )
    {

//        CTPeriod p1("2010-10-10","2012-12-12");
//
//        cout << sizeof(p1) << endl;
//        //REQUIRE( sizeof(p1) == );
//        using namespace boost::interprocess;
//        managed_shared_memory segment(create_only,
//                "MySharedMemory",  //segment name
//                65536);
//
//        //Allocate a buffer in shared memory to write data
//        char *my_cstring = segment.construct<char>("MyCString")[sizeof(p1)+1](0);
//        bufferstream io(my_cstring, sizeof(p1)+1);
//
//        io << p1;
//
//
//          //Check there was no overflow attempt
//        assert(io.good());
//
//
//        CTPeriod p2;
//
//        io.seekp(0, std::ios::beg);
//        io >> p2;
//
//        io.clear();
//
//        REQUIRE( p2 == p1 );

    }
}


TEST_CASE( "UtilTime", "[CTReferencedVector]" )
{

    SECTION( "constructor" )
    {
        CTPeriod p("2024-10-16|2024-10-20");
        CTReferencedVector<float> v(p);
        REQUIRE( v.is_init() );
        REQUIRE( v.size() == p.size() );
    }
}




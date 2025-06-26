std::vector<unsigned> data1( 10 , 1 );
    std::vector<unsigned> data2( 10 , 2 );

    std::ofstream outputFile( "Foo.txt" );
    std::ostream_iterator<DataPair> outputIterator( outputFile , "\n" );

    std::copy(
        boost::make_zip_iterator( boost::make_tuple( data1.begin() , data2.begin() ) ) ,
        boost::make_zip_iterator( boost::make_tuple( data1.end()   , data2.end()   ) ) ,
        outputIterator );

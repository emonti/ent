require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe Ent::RandomTest do
  context "byte entropy" do
    before :each do
      @rt = Ent::RandomTest.new
    end

    it "should not have binary testing enabled" do
      @rt.should_not be_binary
    end

    it "should initialize correctly" do
      @rt.should be_kind_of Ent::RandomTest
      @rt.entropy.should be_nil
      @rt.mean.should be_nil
      @rt.chisquare.should be_nil
      @rt.scc.should be_nil
    end

    it "should default with no randomness" do
      @rt.should be_kind_of Ent::RandomTest
      @rt.entropy.should be_nil
      @rt.mean.should be_nil
      @rt.chisquare.should be_nil
      @rt.chisquare_probability.should be_nil
      @rt.scc.should be_nil

      @rt.finalize

      @rt.entropy.should == 0
      @rt.chisquare_probability.should > 99.99
      @rt.mean.should be_nan
      @rt.chisquare.should be_nan
    end

    it "should read a string and calculate its entropy" do
      @rt.read_string("foo").should == 3
      @rt.finalize
      (0.918295 .. 0.918297).should be_include(@rt.entropy)
      (423.66 .. 423.67).should be_include(@rt.chisquare)
      (108.0000 .. 108.0001).should be_include(@rt.mean)
      @rt.chisquare_probability.should > 99.99
    end

    it "should read a file and calculate its entropy" do
      @rt.read_file(sample_file("foo_test")).should == 3
      @rt.finalize
      (0.918295 .. 0.918297).should be_include(@rt.entropy)
      (423.66 .. 423.67).should be_include(@rt.chisquare)
      (108.0000 .. 108.0001).should be_include(@rt.mean)
      @rt.chisquare_probability.should > 99.99
    end

  end

  context "binary entropy" do
    before :each do
      @rt = Ent::RandomTest.new(true)
    end


    it "should have binary testing enabled" do
      @rt.should be_binary
    end

    it "should initialize correctly" do
      @rt.should be_kind_of Ent::RandomTest
      @rt.entropy.should be_nil
      @rt.mean.should be_nil
      @rt.chisquare.should be_nil
      @rt.scc.should be_nil
    end

    it "should default with no randomness" do
      @rt.should be_kind_of Ent::RandomTest
      @rt.entropy.should be_nil
      @rt.mean.should be_nil
      @rt.chisquare.should be_nil
      @rt.scc.should be_nil

      @rt.finalize

      @rt.entropy.should == 0
      @rt.chisquare_probability.should < 0.01
      @rt.mean.should be_nan
      @rt.chisquare.should be_nan
    end

    it "should read a string and calculate its entropy" do
      @rt.read_string("foo").should == 3
      @rt.finalize
      (0.918295 .. 0.918297).should be_include(@rt.entropy)
      (2.66 .. 2.67).should be_include(@rt.chisquare)
      (0.6666 .. 0.6667).should be_include(@rt.mean)
      @rt.chisquare_probability.should < 0.01
    end

    it "should read a file and calculate its entropy" do
      @rt.read_file(sample_file("foo_test")).should == 3
      @rt.finalize
      (0.918295 .. 0.918297).should be_include(@rt.entropy)
      (2.66 .. 2.67).should be_include(@rt.chisquare)
      (0.6666 .. 0.6667).should be_include(@rt.mean)
      @rt.chisquare_probability.should < 0.01
    end
  end
end


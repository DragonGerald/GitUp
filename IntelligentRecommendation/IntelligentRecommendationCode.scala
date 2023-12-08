package test

import org.apache.spark.ml.recommendation.ALS
import org.apache.spark.sql.functions._
import org.apache.spark.sql.expressions.Window
import org.apache.spark.ml.evaluation.RegressionEvaluator
import org.apache.spark.sql.functions.{count, desc, format_number, sum}
import org.apache.spark.sql.{DataFrame, SaveMode, SparkSession}
import org.apache.spark.ml.feature.StringIndexer
import org.apache.spark.sql.functions.{col, expr}
import org.apache.spark.sql.DataFrame

object meal {
    def main(args: Array[String]): Unit = {
      //  实例化spark
      val spark = SparkSession.builder()
        //  开启hive连接服务
        .enableHiveSupport()
        // 设置一些 Spark 配置选项
        .config("some.spark.config.options", "some.config")
        .master("local[3]") // 设置 Spark 运行模式为本地，使用3个线程
        .appName("explore") // 设置应用程序名称
        .getOrCreate()
      //  设置日志输出等级为Error
      spark.sparkContext.setLogLevel("Error")
      //  获取数据表 "law.law_visit_log_all"
      val MealRatingsdata = spark
        .read.json("D:\\资料\\1-3周毕业实习\\餐饮大数据智能推荐\\数据\\MealRatings_201705_201706.json")
      print("菜品评论数据集的行数:")
      println(MealRatingsdata.count()) // 输出数据集的行数
      print("菜品评论数据集的结构:")
      MealRatingsdata.printSchema() // 打印数据集的结构
      println("显示原始数据集:")
      MealRatingsdata.show(10)


      // 剔除任何一个属性有缺失值的行
      val MealRatingslostData = MealRatingsdata.na.drop(Seq("UserID", "Rating", "ReviewTime", "Review", "MealID"))

      // 计算处理后的数据行数
      val lostrowCount = MealRatingslostData.count()

      //缺失属性的行数
      val lostnum=MealRatingsdata.count()-lostrowCount
      println(s"处理的缺失行数: $lostnum")
      println(s"处理后的数据行数: $lostrowCount")
      // 打印处理后的数据集
      println(s"处理后的数据集:")
      MealRatingslostData.show(10)

      import spark.implicits._

      // 创建窗口按照 ReviewTime 降序排列
      val windowSpec = Window.partitionBy($"UserID", $"MealID").orderBy($"ReviewTime".desc)

      // 选择具有最大 ReviewTime 的行
      val FinaMealRatingsData = MealRatingslostData.withColumn("row_number", row_number().over(windowSpec))
        .filter($"row_number" === 1)
        .drop("row_number")

      // 去除重复的行
      val processedrepeData = FinaMealRatingsData.dropDuplicates(Seq("UserID", "MealID"))
      val ReperowCount = processedrepeData.count()
      //缺失属性的行数
      val repenum=MealRatingsdata.count()-ReperowCount
      println(s"处理的重复行数: $repenum")
      println(s"处理的重复数据集：")
      processedrepeData.show()
      println(s"处理后的数据行数: $ReperowCount")
      // 打印处理后的数据集
      println(s"处理后的数据集: ")
      FinaMealRatingsData.show()//显示最终的菜品评论数据集

      // 创建StringIndexer并转换MealID列
      val MealIdIndexer= new StringIndexer().setInputCol("MealID").setOutputCol("MealIDIndex")
      val indexedData1 = MealIdIndexer.fit(FinaMealRatingsData).transform(FinaMealRatingsData)

      // 创建StringIndexer并转换UserID列
      val userIdIndexer = new StringIndexer().setInputCol("UserID").setOutputCol("UserIDIndex")
      val indexedData2 = userIdIndexer.fit(indexedData1).transform(indexedData1)
      //并保存到hive表中
      indexedData2.write.mode("overwrite").format("parquet").saveAsTable("rating_data_table")

      println("转码后的数据集：")
      indexedData2.show()
      // 查询并显示写入Hive表后的内容
      val ratingtableData= spark.sql("SELECT * FROM rating_data_table")
      println("写入Hive表rating_data_table后的内容：")
      ratingtableData.show()

      //读取MySQL数据库中的meal_list表格内容
      val MealListData: DataFrame = spark.read
        .format("jdbc") //指定数据源格式为JDBC
        .option("url", "jdbc:mysql://master:3306") //指定数据库连接URL
        .option("driver", "com.mysql.cj.jdbc.Driver") //指定JDBC驱动程序类
        .option("dbtable", "data.meal_list") //指定要读取的数据表名称
        .option("user", "root") //指定数据库用户名
        .option("password", "123456") //指定数据库密码
        .load() //加载数据
      print("菜品名数据集的行数:")
      println(MealListData.count())  // 输出数据集的行数
      print("菜品名数据集的结构:")
      MealListData.printSchema()  // 打印数据集的结构
      MealListData.show(10)//默认只显示20行

      //挑选有异常的数据行
      val randData = MealListData.filter(col("mealno").isNull || col("mealID").isNull || col("meal_name").isNull)

      // 删除具有任意属性为NULL值的行
      val finaMealListData = MealListData
        .filter(col("mealno").isNotNull && col("mealID").isNotNull && col("meal_name").isNotNull)

      // 统计处理后的数据行数
      val rowCount = finaMealListData.count()
      val randnum=MealListData.count()-rowCount
      println(s"处理的数据数: $randnum")
      println(s"处理的数据集:")
      randData.show()

      // 显示处理后的数据行数
      println(s"处理后的数据数: $rowCount")
      println(s"处理后的数据集:")
      finaMealListData.show()//显示最终的菜品名单数据集

      //进行mealID转码成mealIDIndex
      val mealIdIndexer = new StringIndexer().setInputCol("mealID").setOutputCol("mealIDIndex")
      val FinaMealListData= mealIdIndexer.fit(finaMealListData).transform(finaMealListData)

      //并保存到hive表中
      FinaMealListData.write.mode("overwrite").format("parquet").saveAsTable("meal_list_table")

      println("转码后的数据集：")
      FinaMealListData.show()

      // 查询并显示写入Hive表后的内容
      val mealtableData= spark.sql("SELECT * FROM meal_list_table")
      println("写入Hive表meal_list_table后的内容：")
      mealtableData.show()

      // 从 rating_data_table 获取 MealIDIndex 列
      val mealIDIndexFromRating = ratingtableData.select("MealIDIndex")

      // 从 meal_list_table 获取 mealIDIndex 和 meal_name 列
      val mealIDIndexAndName = mealtableData.select("mealIDIndex", "meal_name")

      // 将两个数据框连接为 rating_meal_data
      val ratingMealData: DataFrame = mealIDIndexFromRating
        .join(mealIDIndexAndName, Seq("MealIDIndex"), "left")

      // 将 rating_meal_data 数据框写入 Hive 表（覆盖现有表）
      ratingMealData.write.mode("overwrite").insertInto("rating_meal_data")
      println("写入Hive表后的内容：")
      ratingMealData.show()
      // 计算每个用户的评论次数，并统计每个用户评论的次数和占比。
      val userCount = FinaMealRatingsData.groupBy("UserID").count()
      val totalCount = userCount.agg(sum("count")).first().getLong(0)
      val userStats = userCount.withColumn("percentage", col("count") / totalCount * 100)

      // 打印每个用户评论的次数和占比
      println("每个用户的评论次数和占比：")
      userStats.show()

      // 从表 "rating_data_table" 中读取数据并创建 DataFrame
      val ALSData = spark.read.table("rating_data_table")
      ALSData.show()
      // 将数据集划分为训练集和测试集。
      val Array(trainingData, testData) = ALSData.randomSplit(Array(0.8, 0.2))

      // 将训练集创建并保存到新表MealRatingsDataTrain表中
      trainingData.write.mode(SaveMode.Overwrite).saveAsTable("MealRatingsDataTrain")


      // 设置ALS模型参数。
      val als = new ALS()
        .setMaxIter(10)
        .setRegParam(0.01)
        .setUserCol("UserIDIndex")
        .setItemCol("MealIDIndex")
        .setRatingCol("Rating")

      // 拟合ALS模型。
      val model = als.fit(trainingData)

      // 使用测试集进行预测，并保存结果到DataFrame。
      val predictions = model.transform(testData)

      // 评估模型性能，计算均方误差RMSE。
      val evaluator = new RegressionEvaluator()
        .setMetricName("rmse")
        .setLabelCol("Rating")
        .setPredictionCol("prediction")
      val rmse = evaluator.evaluate(predictions)

      println(s"均方根误差（RMSE）: $rmse")

      // 11.输出模型的用户因子矩阵和菜品因子矩阵。
      val userFactors = model.userFactors
      val mealFactors = model.itemFactors

      println("用户因子矩阵：")
      userFactors.show()

      println("菜品因子矩阵：")
      mealFactors.show()

      // 为所有用户推荐前10个菜品，并输出结果。
      val allUsers = ALSData.select("UserIDIndex").distinct()
      val allUserRecommendations = model.recommendForUserSubset(allUsers, 10)

      println("为所有用户推荐的菜品：")
      allUserRecommendations.show(false)

      // 展开 recommendations 列
      val expandedRecommendations1 = allUserRecommendations.select(
        col("UserIDIndex"),
        explode(col("recommendations")).as("recommendation")
      ).select(
        col("UserIDIndex"),
        col("recommendation.MealIDIndex").alias("MealIDIndex"),
        col("recommendation.rating")
      )

      expandedRecommendations1.show()
      // 将 expandedRecommendations 表与 ratingMealData 表连接
      val joinedResult1= expandedRecommendations1.join(ratingMealData, Seq("MealIDIndex")).dropDuplicates()

      // 打印连接后的结果
      println("为所有用户推荐的菜品及对应的菜名：")
      joinedResult1.select("UserIDIndex", "MealIDIndex", "meal_name").show(false)

      // 为所有菜品推荐前10个用户，并输出结果。
      val allMeals = ALSData.select("MealIDIndex").distinct()
      val allMealRecommendations = model.recommendForItemSubset(allMeals, 10)

      println("为所有菜品推荐的用户：")
      allMealRecommendations.show(false)

      // 展开 recommendations 列
      val expandedRecommendations2= allUserRecommendations.select(
        col("UserIDIndex"),
        explode(col("recommendations")).as("recommendation")
      ).select(
        col("UserIDIndex"),
        col("recommendation.MealIDIndex").alias("MealIDIndex"),
        col("recommendation.rating")
      )

      // 将 expandedRecommendations 表与 ratingMealData 表连接
      val joinedResult2= expandedRecommendations2.join(ratingMealData, Seq("MealIDIndex")).dropDuplicates()
        .dropDuplicates()
      // 打印连接后的结果
      println("为所有菜品推荐的用户及对应的菜名：")
      joinedResult2.select("UserIDIndex", "MealIDIndex", "meal_name").show(false)

      // 为部分用户推荐前10个菜品，并输出结果。
      val someUsers = ALSData.select("UserIDIndex").distinct().limit(5)
      val someUserRecommendations = model.recommendForUserSubset(someUsers, 10)

      println("为部分用户推荐的菜品：")
      someUserRecommendations.show(false)

      val expandedRecommendations3= allUserRecommendations.select(
        col("UserIDIndex"),
        explode(col("recommendations")).as("recommendation")
      ).select(
        col("UserIDIndex"),
        col("recommendation.MealIDIndex").alias("MealIDIndex"),
        col("recommendation.rating")
      )

      // expandedRecommendations 表与 ratingMealData 表连接
      val joinedResult3= expandedRecommendations3.join(ratingMealData, Seq("MealIDIndex")).dropDuplicates()

      // 打印连接后的结果
      println("为部分用户推荐的菜品：")
      joinedResult3.select("UserIDIndex", "MealIDIndex", "meal_name").show(false)

      // 为部分菜品推荐前10个用户，并输出结果。
      val someMeals = ALSData.select("MealIDIndex").distinct().limit(5)
      val someMealRecommendations = model.recommendForItemSubset(someMeals, 10)

      println("为部分菜品推荐的用户：")
      someMealRecommendations.show(false)
      val expandedRecommendations4= allUserRecommendations.select(
        col("UserIDIndex"),
        explode(col("recommendations")).as("recommendation")
      ).select(
        col("UserIDIndex"),
        col("recommendation.MealIDIndex").alias("MealIDIndex"),
        col("recommendation.rating")
      )

      // 将 expandedRecommendations 表与 ratingMealData 表连接
      val joinedResult4= expandedRecommendations4.join(ratingMealData, Seq("MealIDIndex")).dropDuplicates()

      // 打印连接后的结果
      println("为部分菜品推荐的用户：")
      joinedResult4.select("UserIDIndex", "MealIDIndex", "meal_name").show(false)
  }
}

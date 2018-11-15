package com.konan.utility;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.konan.utility.CommonUtil;
import com.konan.vo.CommonPagingVO;
import com.konan.vo.CommonSearchVO;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
import net.sf.json.JsonConfig;
import net.sf.json.processors.DefaultValueProcessor;

/**
* 
* Comment	: 공통으로 자주 쓰이는 함수의 집합군 Class
* 
**/
public class CommonUtil {
	
	/**
	* Method		 : isEmpty
	* Comment		 : 넘겨받은 Object에 value값이 존재 하는지 여부를 판단하여 true, false를 리턴한다.
	* Modify Memo : 
	**/
	@SuppressWarnings("rawtypes")
	public static boolean isEmpty(Object obj) {
		if(obj == null) { return true; }
		else if(obj instanceof Date) { return false; }
		else if(obj instanceof String) { if("".equals(((String)obj).trim())) return true; }
		else if(obj instanceof Integer) { return false; }
		else if(obj instanceof Long) { return false; }
		else if(obj instanceof Float) { return false; }
		else if(obj instanceof Double) { return false; }
		else if(obj instanceof Boolean) { return false; }
		else if(obj instanceof Map) { return ((Map)obj).isEmpty(); }
		else if(obj instanceof List) { 
			for (Object v : ((List)obj)) {
				if(!isEmpty(v)) {return false;}
			}
			return true;  
		} else if(obj instanceof Object[]) { 
			for (Object v : ((Object[])obj)) {
				if(!isEmpty(v)) {return false;}
			}
			return true; 
		} else {
			Class cls = obj.getClass();
			List<Field> fieldList = new ArrayList<Field>();
			fieldList = getAllFields(cls, fieldList);
			for(Field F : fieldList){
				try {
					if(F.getModifiers() == 1) { if(!isEmpty(F.get(obj))) return false; }
					else if(F.getModifiers() == 2) { if(!isEmpty(getFieldValue(obj, F.getName()))) return false; }
					else if(F.getModifiers() == 18) { if(!isEmpty(getFieldValue(obj, F.getName()))) return false; }
				} 
				catch (Exception e) {}
			}
			return true;
		}
		return false;
	}
	
	/**
	* Method		 : getAllFields
	* Comment		 : 모든 멤버필드를 반환한다.
	* Modify Memo : 
	**/
	@SuppressWarnings("rawtypes")
	public static List<Field> getAllFields(Class cls, List<Field> fields) {
	    fields.addAll(Arrays.asList(cls.getDeclaredFields()));
	    if (cls.getSuperclass() != null) {
	        fields = getAllFields(cls.getSuperclass(), fields);
	    }
	    return fields;
	}
	
	/**
	* Method		 : getFieldValue
	* Comment		 : 멤버필드의 Getter 메소드를 이용하여 값을 리턴
	* Modify Memo : 
	**/
	@SuppressWarnings("rawtypes")
	public static Object getFieldValue(Object object, String fieldName){
		try {
			Class cls = object.getClass();			
			HashMap<String, HashMap<String, Object>> classGetterMethods = getClassGetter(cls, null);
			if(!CommonUtil.isEmpty(classGetterMethods.get(fieldName.toUpperCase()))){
				Method method = (Method)classGetterMethods.get(fieldName.toUpperCase()).get("Method");
				method.setAccessible(true);
				Object returnObject = method.invoke(object);
				method.setAccessible(false);				
				return returnObject;// returnValue != null && returnValue==0)?null:returnValue;
			}else{
				return null;
			}
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}		
	}
	
	/**
	* Method		 : getClassGetter
	* Comment		 : 해당 Class내의 Getter 메소드만 추출하여 메소드와 해당 메소드 수행시에 넘겨받을 parameter의 Object Type을 HashMap으로 반환한다.
	* Modify Memo : 
	**/
	@SuppressWarnings("rawtypes")
	public static HashMap<String, HashMap<String, Object>> getClassGetter(Class cls, HashMap<String, HashMap<String, Object>> methods){
		HashMap<String, HashMap<String, Object>> returnMethods = new HashMap<String, HashMap<String, Object>>();
		HashMap<String, Class> fields = new HashMap<String, Class>();
		if(!CommonUtil.isEmpty(methods)) returnMethods = methods;
		Method[] methodArr = cls.getMethods();
		Field[] fieldArr = cls.getFields();
		
		for(Field F : fieldArr){ fields.put(F.getName().toUpperCase(), F.getType());}
		for(Method M : methodArr){
			if(M.getName().startsWith("get")){
				String FieldName = M.getName().substring(3).toUpperCase();
				HashMap<String, Object> info = new HashMap<String, Object>();
				info.put("Method", M);
				info.put("ParamType", fields.get(FieldName));
				returnMethods.put(FieldName, info);
			}
		}
		return returnMethods;
		/*
		if(cls.equals(Object.class)) return returnMethods;
		return getClassGetter(cls.getSuperclass(), returnMethods);
		*/
	}	
	
	public static CommonPagingVO getPagingInfo(CommonSearchVO vo){
		return getPagingInfo((CommonPagingVO)vo);
	}
	
	public static CommonPagingVO getPagingInfo(CommonPagingVO vo){
		CommonPagingVO paging = new CommonPagingVO();
		paging.setPageNo(vo.getPageNo());
		paging.setTotalCount(vo.getTotalCount());		
		paging.setPageSize(vo.getPageSize());
		paging.setWindowCount(vo.getWindowCount());		
		
		return paging;
	}	

	public static Object parseToJson(Object object){
		if(object instanceof List || object instanceof Object[]){
			return JSONArray.fromObject(object, jsonConfig());
		}else{
			return JSONObject.fromObject(object, jsonConfig());
		}
	}
	
	/**
	* Method		 : makeParams
	* Comment		 : 콤마(,)형태로 구분되어져서 넘어온 객체를 순서대로 Map 형태로 변환한다.
	* Modify Memo : 
	**/
	public static HashMap<String, Object> makeParams(Object... objects){
		HashMap<String, Object> params = new HashMap<String, Object>(2);	
		int size = objects.length;
		for(int i = 0; i < size; i += 2){
			params.put((String)objects[i], CommonUtil.isEmpty(objects[i+1])?null:objects[i+1]);
		}
		return params;
	}
	
	@SuppressWarnings("rawtypes")
	private static JsonConfig jsonConfig() {
		JsonConfig config = new JsonConfig();
		config.registerDefaultValueProcessor(String.class, new DefaultValueProcessor() {
			@Override
			public Object getDefaultValue(Class arg0) {return "";}
		});
		config.registerDefaultValueProcessor(Date.class, new DefaultValueProcessor() {
			@Override
			public Object getDefaultValue(Class arg0) {return "";}
		});
		config.registerDefaultValueProcessor(Integer.class, new DefaultValueProcessor() {
			@Override
			public Object getDefaultValue(Class arg0) {return "";}
		});
		
		return config;
	}	
}
